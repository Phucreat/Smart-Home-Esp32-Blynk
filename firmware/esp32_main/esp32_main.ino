/**
 * @file esp32_main.ino
 * @brief Smart Home Central Automation & IoT Controller (ESP32)
 * @author Nguyễn Văn Phú (MSSV: 23021322) - EMA 2027 Mechatronics
 * @version 1.0.0
 * 
 * Chức năng chính:
 * 1. Giám sát môi trường: Nhiệt độ, độ ẩm (DHT11), Cảm biến mưa.
 * 2. Cảnh báo an ninh & an toàn cháy nổ: Lửa (Flame Sensor), Khí Gas rò rỉ (MQ-2).
 * 3. Kiểm soát ra vào tự động (Access Control): RFID-RC522 + Động cơ Servo.
 * 4. Điều khiển giàn phơi thông minh: Tự động kéo/thu dựa trên cảm biến mưa.
 * 5. Điều khiển chiếu sáng đa phòng & thiết bị qua App Blynk IoT & Màn hình OLED SSD1306.
 */

// Đảm bảo các định nghĩa Blynk Template nằm trước thư viện Blynk
#define BLYNK_TEMPLATE_ID   "TMPL4itusNubY"
#define BLYNK_TEMPLATE_NAME "SMARTHOME"
#define BLYNK_AUTH_TOKEN    "BPRTxSexCfoaSZ11I8J_LMcpHQBiHWJ7"

// Thư viện cần thiết
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

// ====================================================================
//                   CẤU HÌNH THIẾT BỊ & CHÂN KẾT NỐI (PINOUT)
// ====================================================================
// Thông tin WiFi
char ssid[] = "TP-Link_9F36";
char pass[] = "38361623";

// Cảm biến
const int PIN_FLAME = 35;       // Cảm biến lửa
const int PIN_GAS   = 34;       // Cảm biến Gas MQ-2 (Analog)
const int PIN_RAIN  = 32;       // Cảm biến mưa
#define PIN_DHT       14        // Cảm biến DHT11
#define DHTTYPE       DHT11

// Relay điều khiển thiết bị điện (Kích mức LOW)
const int RELAY_LIVING_ROOM = 33; // Đèn phòng khách
const int RELAY_KITCHEN     = 25; // Đèn phòng bếp
const int RELAY_BEDROOM     = 26; // Đèn phòng ngủ
const int RELAY_ALARM_FAN   = 27; // Chuông còi & Quạt hút khẩn cấp

// Giàn phơi thông minh (Động cơ DC đảo chiều)
const int PIN_MOC_1 = 13;
const int PIN_MOC_2 = 12;

// Cửa tự động (RFID RC522 & Servo SG90)
#define PIN_SERVO   4
#define SS_PIN      5           // Chân SDA/SS của RFID
#define RST_PIN     15          // Chân RST của RFID

// Màn hình OLED I2C (SSD1306 128x64)
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ====================================================================
//                   KHỞI TẠO ĐỐI TƯỢNG VÀ BIẾN TOÀN CỤC
// ====================================================================
DHT dht(PIN_DHT, DHTTYPE);
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo;

// Danh sách mã UID thẻ từ RFID hợp lệ
const char *allowedUIDs[] = {
  "63 88 4A AC"  // Thẻ Master
};
const int allowedCount = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]);

// Biến trạng thái thiết bị
bool ledPk      = false;
bool ledPn      = false;
bool ledPb      = false;
bool bell       = false;
bool autodry    = false;
String message  = "";

// Biến trạng thái cảm biến & an ninh
bool flameAlert = false;
bool gasAlert   = false;
bool rainAlert  = false;
bool hasPerson  = false;
unsigned long lastPersonDetect = 0;

// Trạng thái giàn phơi
bool isRaining   = false;
bool hasExtended = false;

// ====================================================================
//                   XỬ LÝ SỰ KIỆN TỪ BLYNK IOT APP
// ====================================================================
BLYNK_WRITE(V0) { ledPk = param.asInt(); }    // Đèn phòng khách
BLYNK_WRITE(V1) { ledPb = param.asInt(); }    // Đèn phòng bếp
BLYNK_WRITE(V2) { ledPn = param.asInt(); }    // Đèn phòng ngủ
BLYNK_WRITE(V3) { bell = param.asInt(); }     // Còi chuông / Quạt
BLYNK_WRITE(V8) { autodry = param.asInt(); }  // Chế độ giàn phơi
BLYNK_WRITE(V10) { message = param.asStr(); } // Tin nhắn hiển thị OLED

// ====================================================================
//                   CÁC MODULE CHỨC NĂNG HỆ THỐNG
// ====================================================================

/**
 * @brief Điều khiển bật/tắt thiết bị gia dụng qua Relay
 */
void controlDevice() {
  digitalWrite(RELAY_LIVING_ROOM, ledPk ? LOW : HIGH);
  digitalWrite(RELAY_KITCHEN,     ledPb ? LOW : HIGH);
  digitalWrite(RELAY_BEDROOM,     ledPn ? LOW : HIGH);
  digitalWrite(RELAY_ALARM_FAN,   bell  ? LOW : HIGH);
}

/**
 * @brief Giám sát cảm biến mưa & Tự động đóng/mở giàn phơi thông minh
 */
void handleAutoClothesline() {
  int rainState = digitalRead(PIN_RAIN); // LOW = Có mưa
  Blynk.virtualWrite(V8, rainState);

  if (rainState == LOW) { // Phát hiện trời mưa -> Thu giàn phơi vào
    if (!isRaining) {
      rainAlert = true;
      digitalWrite(PIN_MOC_1, LOW);
      digitalWrite(PIN_MOC_2, HIGH);
      delay(2000);
      digitalWrite(PIN_MOC_1, LOW);
      digitalWrite(PIN_MOC_2, LOW);
      isRaining = true;
      hasExtended = false;
    }
  } else { // Trời tạnh mưa -> Kéo giàn phơi ra ngoài
    if (isRaining && !hasExtended) {
      rainAlert = false;
      digitalWrite(PIN_MOC_1, HIGH);
      digitalWrite(PIN_MOC_2, LOW);
      delay(1000);
      digitalWrite(PIN_MOC_1, LOW);
      digitalWrite(PIN_MOC_2, LOW);
      hasExtended = true;
      isRaining = false;
    }
  }
}

/**
 * @brief Hệ thống báo cháy và rò rỉ khí gas khẩn cấp
 */
void handleSafetyAlarm() {
  int flameSignal = digitalRead(PIN_FLAME); // HIGH = Phát hiện lửa
  int gasSignal   = analogRead(PIN_GAS);    // Giá trị ADC nồng độ khí gas

  Blynk.virtualWrite(V5, flameSignal);
  Blynk.virtualWrite(V4, gasSignal);

  flameAlert = (flameSignal == HIGH);
  gasAlert   = (gasSignal > 1000);

  // Kích hoạt còi báo động khẩn cấp nếu có cháy hoặc rò rỉ khí gas
  if (flameAlert || gasAlert) {
    digitalWrite(RELAY_ALARM_FAN, LOW); // Bật còi & quạt hút
  } else if (!bell) {
    digitalWrite(RELAY_ALARM_FAN, HIGH); // Tắt còi khi an toàn (nếu Blynk không bật)
  }
}

/**
 * @brief Đọc thông số nhiệt độ & độ ẩm môi trường và gửi lên Blynk
 */
void handleEnvironmentSensors() {
  float humid = dht.readHumidity();
  float temp  = dht.readTemperature();

  if (!isnan(humid) && !isnan(temp)) {
    Blynk.virtualWrite(V6, temp);
    Blynk.virtualWrite(V7, humid);
  }
}

/**
 * @brief Hiển thị tin nhắn thông báo lên màn hình OLED
 */
void displayMessage(const char* msg) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}

/**
 * @brief Cập nhật bảng thông số thời gian thực lên màn hình OLED
 */
void updateOLEDDisplay() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Kiểm tra thời gian hiển thị trạng thái người vào
  if (hasPerson && (millis() - lastPersonDetect > 5000)) {
    hasPerson = false;
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  // Trạng thái WiFi
  display.print(F("WiFi: "));
  display.println(WiFi.status() == WL_CONNECTED ? F("OK") : F("MAT KET NOI"));

  // Trạng thái cửa / Người
  display.print(F("Nguoi: "));
  display.println(hasPerson ? F("VUA VAO") : F("---"));

  // Cảnh báo an ninh
  display.print(F("Lua: "));
  display.println(flameAlert ? F("CO CHAY!") : F("An toan"));

  display.print(F("Gas: "));
  display.println(gasAlert ? F("RO RI!") : F("An toan"));

  display.print(F("Mua: "));
  display.println(digitalRead(PIN_RAIN) == LOW ? F("Troi Mua") : F("Khong"));

  // Thông số môi trường
  display.print(F("Nhiet do: "));
  display.print(isnan(t) ? 0 : t, 1);
  display.println(F(" C"));

  display.print(F("Do am: "));
  display.print(isnan(h) ? 0 : h, 1);
  display.println(F(" %"));

  if (message.length() > 0) {
    display.print(F("Msg: "));
    display.println(message);
  }

  display.display();
}

/**
 * @brief Kiểm tra thẻ từ RFID và tự động điều khiển mở/đóng cửa
 */
void handleAutoDoorRFID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Chuyển đổi mã UID sang chuỗi HEX viết hoa
  String readUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) readUID += "0";
    readUID += String(mfrc522.uid.uidByte[i], HEX);
    if (i != mfrc522.uid.size - 1) readUID += " ";
  }
  readUID.toUpperCase();

  // Xác thực quyền truy cập
  bool authorized = false;
  for (int i = 0; i < allowedCount; i++) {
    if (readUID == allowedUIDs[i]) {
      authorized = true;
      break;
    }
  }

  if (authorized) {
    hasPerson = true;
    lastPersonDetect = millis();
    displayMessage(">> The Hop Le <<\n  Dang Mo Cua...");
    myServo.write(0);        // Mở cửa
    delay(3000);             // Giữ cửa mở trong 3 giây
    myServo.write(90);       // Đóng cửa lại
    displayMessage(">> Da Dong Cua <<");
  } else {
    displayMessage(">> Canh Bao <<\n The Khong Hop Le!");
    delay(1500);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// ====================================================================
//                   SETUP & MAIN LOOP
// ====================================================================
void setup() {
  Serial.begin(115200);

  // Cấu hình chân I/O
  pinMode(PIN_FLAME, INPUT);
  pinMode(PIN_GAS, INPUT);
  pinMode(PIN_RAIN, INPUT);

  pinMode(RELAY_LIVING_ROOM, OUTPUT);
  pinMode(RELAY_KITCHEN, OUTPUT);
  pinMode(RELAY_BEDROOM, OUTPUT);
  pinMode(RELAY_ALARM_FAN, OUTPUT);

  pinMode(PIN_MOC_1, OUTPUT);
  pinMode(PIN_MOC_2, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);

  // Mặc định tắt các Relay (Active LOW)
  digitalWrite(RELAY_LIVING_ROOM, HIGH);
  digitalWrite(RELAY_KITCHEN, HIGH);
  digitalWrite(RELAY_BEDROOM, HIGH);
  digitalWrite(RELAY_ALARM_FAN, HIGH);
  digitalWrite(PIN_MOC_1, LOW);
  digitalWrite(PIN_MOC_2, LOW);

  // Khởi động OLED I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("[ERROR] OLED Khong khoi dong duoc!"));
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    displayMessage("Khoi dong he thong...\nDang ket noi WiFi...");
  }

  // Khởi động giao tiếp WiFi & Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  displayMessage("Da ket noi Blynk!\nSan sang hoat dong.");

  // Khởi động cảm biến DHT11
  dht.begin();

  // Khởi động SPI & RFID Module
  SPI.begin();
  mfrc522.PCD_Init();

  // Khởi động Servo mở cửa
  myServo.attach(PIN_SERVO, 500, 2400);
  myServo.write(90); // Vị trí đóng cửa ban đầu
}

void loop() {
  Blynk.run();
  handleSafetyAlarm();
  handleEnvironmentSensors();
  handleAutoDoorRFID();
  controlDevice();
  handleAutoClothesline();
  updateOLEDDisplay();
}

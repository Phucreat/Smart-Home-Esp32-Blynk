#ifndef CONFIG_H
#define CONFIG_H

// ====================================================================
//                   BLYNK IOT CONFIGURATION
// ====================================================================
#define BLYNK_TEMPLATE_ID   "TMPL4itusNubY"
#define BLYNK_TEMPLATE_NAME "SMARTHOME"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN_HERE"

// ====================================================================
//                   WIFI CREDENTIALS
// ====================================================================
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASS[] = "YOUR_WIFI_PASSWORD";

// ====================================================================
//                   HARDWARE PIN DEFINITIONS (ESP32)
// ====================================================================
// --- Sensor Pins ---
const int PIN_FLAME_SENSOR = 35; // Cảm biến lửa (Flame Sensor - Digital)
const int PIN_GAS_SENSOR   = 34; // Cảm biến khí Gas / Khói (MQ-2 - Analog)
const int PIN_RAIN_SENSOR  = 32; // Cảm biến mưa (Rain Sensor - Digital)
#define PIN_DHT            14    // Cảm biến nhiệt độ, độ ẩm (DHT11)
#define DHT_TYPE           DHT11

// --- Actuator / Relay Pins (Active LOW) ---
const int PIN_RELAY_LIVING_ROOM = 33; // Đèn phòng khách
const int PIN_RELAY_KITCHEN     = 25; // Đèn phòng bếp
const int PIN_RELAY_BEDROOM     = 26; // Đèn phòng ngủ
const int PIN_RELAY_ALARM_FAN   = 27; // Còi báo động & Quạt hút

// --- RFID RC522 (SPI) Pins ---
#define PIN_RFID_SS        5     // SDA / SS Pin
#define PIN_RFID_RST       15    // RST Pin
// SPI Default ESP32: MOSI = 23, MISO = 19, SCK = 18

// --- Servo Door & Motor Pins ---
#define PIN_SERVO_DOOR     4     // Chân tín hiệu điều khiển Servo mở cửa
const int PIN_MOTOR_DRY_1  = 13; // Động cơ giàn phơi IN1 / H-Bridge
const int PIN_MOTOR_DRY_2  = 12; // Động cơ giàn phơi IN2 / H-Bridge

// --- OLED 0.96" I2C Pins ---
#define SCREEN_WIDTH       128
#define SCREEN_HEIGHT      64
#define OLED_RESET         -1
#define OLED_I2C_ADDR      0x3C
// I2C Default ESP32: SDA = 21, SCL = 22

// ====================================================================
//                   RFID ACCESS CONTROL LIST
// ====================================================================
// Danh sách các mã UID thẻ từ được cấp quyền mở cửa (HEX format)
const char* const ALLOWED_UIDS[] = {
    "63 88 4A AC"
    // Thêm các thẻ khác tại đây nếu cần:
    // "DE AD BE EF",
    // "04 A2 1C 3B"
};
const int ALLOWED_UID_COUNT = sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]);

// ====================================================================
//                   THRESHOLD & TIMING PARAMETERS
// ====================================================================
const int GAS_THRESHOLD          = 1000; // Ngưỡng phát hiện rò rỉ khí gas (ADC)
const unsigned long DOOR_OPEN_MS = 3000; // Thời gian mở cửa trước khi tự đóng (ms)

#endif // CONFIG_H

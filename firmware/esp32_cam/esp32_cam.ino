/**
 * @file esp32_cam.ino
 * @brief ESP32-CAM Smart Home Surveillance & Live Web Video Streamer
 * @author Nguyễn Văn Phú (MSSV: 23021322) - EMA 2027 Mechatronics
 * @version 1.0.0
 * 
 * Hardware: AI-Thinker ESP32-CAM (OV2640)
 * Library: esp32cam by yoursunny (https://github.com/yoursunny/esp32cam)
 * 
 * Lưu ý khi nạp code:
 * - Nối chân GPIO 0 vào GND để vào chế độ Flash Mode.
 * - Sau khi nạp xong, rút chân GPIO 0 khỏi GND và nhấn nút RESET.
 * - Mở Serial Monitor với baudrate 115200 để lấy địa chỉ IP Web Server.
 */

#include "WifiCam.hpp"
#include <WiFi.h>

// Thông tin kết nối mạng WiFi
static const char* WIFI_SSID = "TP-Link_9F36";
static const char* WIFI_PASS = "38361623";

esp32cam::Resolution initialResolution;
WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();
  esp32cam::setLogger(Serial);
  delay(1000);

  // Cấu hình WiFi Station Mode
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println(F("[INFO] Dang ket noi WiFi cho Camera..."));
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("[ERROR] Ket noi WiFi that bai, ma loi: %d\n", WiFi.status());
    delay(5000);
    ESP.restart();
  }
  Serial.println(F("[SUCCESS] WiFi da ket noi thanh cong!"));

  // Khởi tạo Camera module AI-Thinker OV2640
  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println(F("[ERROR] Khoi tao Camera that bai!"));
      delay(5000);
      ESP.restart();
    }
    Serial.println(F("[SUCCESS] Khoi tao Camera thanh cong!"));
  }

  // Khởi động Streaming Web Server
  Serial.println(F("[INFO] Camera Web Server dang khoi dong..."));
  Serial.print(F(">> Truy cap Camera tai: http://"));
  Serial.println(WiFi.localIP());

  addRequestHandlers();
  server.begin();
}

void loop() {
  server.handleClient();
}

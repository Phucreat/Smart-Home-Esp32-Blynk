# 🔌 ESP32 Pinout & Wiring Mapping

Tài liệu chi tiết về sơ đồ kết nối chân (Pinout Mapping) giữa **ESP32 Dev Module** với tất cả các module ngoại vi, cảm biến và cơ cấu chấp hành.

---

## 1. Bảng Phân Bổ Chân GPIO (ESP32 Master Controller)

```
                       +------------------------+
                       |      ESP32 DEV KIT     |
                       |                        |
             3V3 ------| 3V3                GND |------ GND (Chung hệ thống)
              EN ------| EN                GPIO23 |------ RFID MOSI
  MQ-2 Gas (A0) -------| GPIO36(VP)        GPIO22 |------ OLED SCL (I2C)
   Flame Sensor -------| GPIO35(VN)         GPIO1 |------ TX0 (Serial Debug)
                       | GPIO34            GPIO3 |------ RX0 (Serial Debug)
    Rain Sensor -------| GPIO32            GPIO21 |------ OLED SDA (I2C)
Relay 1 (Khách) -------| GPIO33            GPIO19 |------ RFID MISO
Relay 2 (Bếp)   -------| GPIO25            GPIO18 |------ RFID SCK
Relay 3 (Ngủ)   -------| GPIO26             GPIO5 |------ RFID SDA/SS
Relay 4 (Còi)   -------| GPIO27             GPIO4 |------ Servo Signal
  DC Motor IN1  -------| GPIO13             GPIO0 |------ Boot
  DC Motor IN2  -------| GPIO12             GPIO2 |------ LED Built-in
   DHT11 Data   -------| GPIO14            GPIO15 |------ RFID RST
                       +------------------------+
```

---

## 2. Chi Tiết Kết Nối Từng Phân Hệ

### 2.1. Cảm Biến Giám Sát & An Ninh (Sensors)

| Linh Kiện | Chân Linh Kiện | Chân ESP32 | Loại Tín Hiệu | Mô Tả |
| :--- | :--- | :--- | :--- | :--- |
| **DHT11** | VCC | 3.3V / 5V | Nguồn | Nguồn cấp cảm biến |
| | GND | GND | Nguồn | Nối đất |
| | DATA | **GPIO14** | Digital (1-Wire) | Đọc nhiệt độ và độ ẩm |
| **Flame Sensor** | VCC | 3.3V / 5V | Nguồn | Nguồn cấp |
| | GND | GND | Nguồn | Nối đất |
| | D0 | **GPIO35** | Digital Input | HIGH khi có ngọn lửa |
| **MQ-2 Gas Sensor** | VCC | 5V | Nguồn | Nguồn nuôi heater |
| | GND | GND | Nguồn | Nối đất |
| | A0 | **GPIO34** | Analog Input (ADC1) | Đọc điện áp nồng độ gas |
| **Rain Sensor** | VCC | 3.3V / 5V | Nguồn | Nguồn cấp |
| | GND | GND | Nguồn | Nối đất |
| | D0 | **GPIO32** | Digital Input | LOW khi có nước mưa |

---

### 2.2. Kiểm Soát Ra Vào (RFID RC522 & Servo Door)

| Module | Chân Module | Chân ESP32 | Ghi Chú Giao Tiếp |
| :--- | :--- | :--- | :--- |
| **RFID-RC522** | 3.3V | **3.3V** *(Không dùng 5V)* | Điện áp chuẩn logic RFID |
| | GND | GND | Nối đất chung |
| | RST | **GPIO15** | Reset Pin |
| | SDA (SS) | **GPIO5** | SPI Chip Select |
| | SCK | **GPIO18** | SPI Serial Clock |
| | MISO | **GPIO19** | SPI Master In Slave Out |
| | MOSI | **GPIO23** | SPI Master Out Slave In |
| **Servo SG90** | VCC (Dây Đỏ) | 5V (Nguồn ngoài) | Tránh sụt áp ESP32 |
| | GND (Dây Nâu) | GND | Nối đất chung |
| | PWM (Dây Cam) | **GPIO4** | Điều khiển góc quay 0°-90° |

---

### 2.3. Màn Hình OLED 0.96" I2C

| Màn Hình OLED | Chân ESP32 | Mô Tả Giao Tiếp |
| :--- | :--- | :--- |
| VCC | 3.3V / 5V | Nguồn cấp SSD1306 |
| GND | GND | Nối đất chung |
| SCL | **GPIO22** | I2C Clock Line |
| SDA | **GPIO21** | I2C Data Line |

---

### 2.4. Khối Cơ Cấu Chấp Hành & Chiếu Sáng (Relay & Motor)

| Cơ Cấu | Chân Điều Khiển | Chân ESP32 | Mức Kích Hoạt | Thiết Bị Gắn Sau Tiếp Điểm |
| :--- | :--- | :--- | :--- | :--- |
| **Relay 1** | IN1 | **GPIO33** | Active LOW | Đèn chiếu sáng Phòng Khách |
| **Relay 2** | IN2 | **GPIO25** | Active LOW | Đèn chiếu sáng Phòng Bếp |
| **Relay 3** | IN3 | **GPIO26** | Active LOW | Đèn chiếu sáng Phòng Ngủ |
| **Relay 4** | IN4 | **GPIO27** | Active LOW | Còi báo động (Buzzer) & Quạt thông gió |
| **DC Motor Driver** | IN1 | **GPIO13** | Logic H-Bridge | Chiều kéo giàn phơi ra |
| | IN2 | **GPIO12** | Logic H-Bridge | Chiều thu giàn phơi vào |

---

### 2.5. ESP32-CAM (Camera Giám Sát Độc Lập)

| Chân ESP32-CAM | Chân Mạch Nạp FTDI (Khi Nạp Code) | Chân Khi Chạy Thực Tế |
| :--- | :--- | :--- |
| 5V / 3V3 | 5V (Khuyến nghị nguồn 5V-2A ổn định) | 5V (Nguồn chung hệ thống) |
| GND | GND | GND |
| U0R (RX) | TXD của mạch FTDI | Không kết nối |
| U0T (TX) | RXD của mạch FTDI | Không kết nối |
| **GPIO0** | **Nối GND** (Chế độ Flash Code) | **Để hở** (Chế độ Run Server) |

# 📋 Bill of Materials (BOM) - Danh Mục Linh Kiện Phần Cứng

Dưới đây là danh sách toàn bộ linh kiện phần cứng, cảm biến, cơ cấu chấp hành và module điều khiển được sử dụng trong mô hình **ESP32 IoT Smart Home System**.

---

## 1. Vi Điều Khiển & Xử Lý Trung Tâm (Control & Processing Units)

| STT | Tên Linh Kiện | Số Lượng | Thông Số Kỹ Thuật | Vai Trò & Chức Năng |
| :---: | :--- | :---: | :--- | :--- |
| **01** | **ESP32 Dev Module (NodeMCU-32S)** | 1 | Dual-core Tensilica LX6 @ 240MHz, 520KB SRAM, Wi-Fi 802.11 b/g/n, BLE 4.2 | Vi điều khiển trung tâm thu thập dữ liệu cảm biến, kết nối Blynk IoT Server, điều khiển toàn bộ thiết bị chấp hành. |
| **02** | **ESP32-CAM AI-Thinker Module** | 1 | CPU ESP32-S, Cảm biến ảnh OV2640 (2 Megapixel), Tích hợp khe cắm thẻ MicroSD, Wi-Fi | Bộ camera giám sát an ninh độc lập, truyền video stream trực tiếp (Live Stream) qua Web Server nội bộ. |
| **03** | **Module Nạp FTDI USB-to-UART** | 1 | FT232RL / CH340G, 3.3V/5V Logic | Nạp firmware và giao tiếp Serial với module ESP32-CAM. |

---

## 2. Cảm Biến Giám Sát & An Ninh (Sensors & Security)

| STT | Tên Linh Kiện | Số Lượng | Giao Thức / Chân | Vai Trò & Chức Năng |
| :---: | :--- | :---: | :--- | :--- |
| **04** | **DHT11 Sensor** | 1 | Digital (1-Wire) - GPIO14 | Đo nhiệt độ phòng (0-50°C) và độ ẩm không khí (20-90% RH). |
| **05** | **Flame Sensor (Cảm biến lửa)** | 1 | Digital - GPIO35 | Phát hiện ngọn lửa hoặc bước sóng hồng ngoại 760nm - 1100nm để cảnh báo cháy tức thì. |
| **06** | **MQ-2 Gas & Smoke Sensor** | 1 | Analog - GPIO34 | Phát hiện rò rỉ khí gas hóa lỏng (LPG, Propane, Methane) và khói trong khu vực bếp. |
| **07** | **Rain / Water Drop Sensor** | 1 | Digital - GPIO32 | Phát hiện nước mưa đọng trên bề mặt cảm biến để kích hoạt thu giàn phơi tự động. |
| **08** | **MFRC522 RFID Reader** | 1 | SPI (SCK: 18, MISO: 19, MOSI: 23, SS: 5, RST: 15) | Đọc mã thẻ từ 13.56MHz cho hệ thống kiểm soát cửa ra vào bảo mật. |

---

## 3. Cơ Cấu Chấp Hành & Hiển Thị (Actuators & Display)

| STT | Tên Linh Kiện | Số Lượng | Thông Số Kỹ Thuật | Vai Trò & Chức Năng |
| :---: | :--- | :---: | :--- | :--- |
| **09** | **Màn Hình OLED 0.96" I2C** | 1 | SSD1306, 128x64 pixels, I2C Address: 0x3C | Hiển thị thông số môi trường, trạng thái WiFi, trạng thái cửa, cảnh báo nguy hiểm thời gian thực. |
| **10** | **Servo Motor SG90** | 1 | Góc quay 0° - 180°, Điện áp 5V, Lực kéo 1.8 kg-cm | Đóng/mở chốt cửa ra vào khi quẹt đúng thẻ từ RFID. |
| **11** | **Module Relay 4 Kênh 5V (Optocoupler)** | 1 | Cách ly quang Opto, Tiếp điểm 10A/250VAC - 10A/30VDC | Đóng cắt độc lập 3 line đèn chiếu sáng (Khách/Bếp/Ngủ) và 1 line Còi báo động/Quạt hút. |
| **12** | **Động Cơ DC Giàn Phơi + Cầu H L298N/Driver** | 1 | Động cơ giảm tốc DC 5V-12V kèm cơ cấu kéo dây phơi | Tự động kéo ra khi trời nắng và thu vào khi phát hiện trời mưa. |
| **13** | **Còi Báo Động (Buzzer 5V / Loa)** | 1 | Active Buzzer 5V DC | Phát âm thanh cảnh báo âm lượng lớn khi có sự cố cháy hoặc rò rỉ khí gas. |

---

## 4. Nguồn Điện & Phụ Kiện Lắp Đặt (Power & Enclosure)

| STT | Tên Linh Kiện | Số Lượng | Ghi Chú |
| :---: | :--- | :---: | :--- |
| **14** | **Nguồn Tổ Ong / Adapter 5V - 2A/3A** | 1 | Cung cấp nguồn ổn định cho ESP32, Servo, Relay, Động cơ và Cảm biến. |
| **15** | **Khung Mô Hình Nhà Thông Minh 3D** | 1 | Thiết kế và in 3D từ file CAD (`SMARTHOME3D.stl`). |
| **16** | **Dây Nối Breadboard (Jumper Wires)** | 1 bộ | Dây cắm đực-cái, cái-cái phân loại theo màu tín hiệu. |
| **17** | **Thẻ Từ & Móc Khóa RFID Keyfob** | 2 | Thẻ chuẩn 13.56MHz Mifare One. |

# 📱 Hướng Dẫn Cấu Hình Nền Tảng Blynk IoT (Web & Mobile Dashboard)

Tài liệu hướng dẫn chi tiết cách thiết lập **Blynk Cloud (Blynk IoT 2.0)** để đồng bộ, giám sát và điều khiển toàn bộ thiết bị trong mô hình Smart Home.

---

## 1. Thông Tin Mẫu Thiết Bị (Device Template)

- **Template Name**: `SMARTHOME`
- **Hardware**: `ESP32`
- **Connection Type**: `WiFi`
- **Template ID**: `TMPL4itusNubY`

---

## 2. Bảng Cấu Hình Datastreams (Virtual Pins)

Thiết lập các **Datastream** trong mục **Template -> Datastreams** trên giao diện Web Blynk:

| Virtual Pin | Tên Datastream | Kiểu Dữ Liệu | Dải Giá Trị (Range) | Đơn Vị (Unit) | Vai Trò Chức Năng |
| :---: | :--- | :---: | :---: | :---: | :--- |
| **V0** | `Living_Room_Light` | Integer | `0 - 1` | - | Bật/Tắt đèn phòng khách |
| **V1** | `Kitchen_Light` | Integer | `0 - 1` | - | Bật/Tắt đèn phòng bếp |
| **V2** | `Bedroom_Light` | Integer | `0 - 1` | - | Bật/Tắt đèn phòng ngủ |
| **V3** | `Alarm_Buzzer_Fan` | Integer | `0 - 1` | - | Kích hoạt còi báo & quạt hút khẩn cấp |
| **V4** | `Gas_Level` | Integer / Double | `0 - 4095` | ppm / ADC | Giám sát nồng độ khí gas từ MQ-2 |
| **V5** | `Flame_Status` | Integer | `0 - 1` | - | Cảnh báo cháy từ Flame Sensor |
| **V6** | `Temperature` | Double | `-40.0 - 80.0` | °C | Giám sát nhiệt độ phòng từ DHT11 |
| **V7** | `Humidity` | Double | `0.0 - 100.0` | % | Giám sát độ ẩm không khí từ DHT11 |
| **V8** | `Rain_Status` | Integer | `0 - 1` | - | Trạng thái mưa & giám sát giàn phơi |
| **V10** | `Display_Message` | String | Tối đa 255 ký tự | - | Gửi thông điệp văn bản lên màn hình OLED |

---

## 3. Giao Diện Điều Khiển (Mobile App Dashboard)

![Giao diện Blynk](../assets/images/blynk_dashboard.jpg)

### Danh Sách Widget Cần Thêm Trên Ứng Dụng Mobile:
1. **Switch / Button Widgets**:
   - Gán với **V0**, **V1**, **V2** (Điều khiển đèn chiếu sáng các phòng độc lập).
   - Gán với **V3** (Bật/Tắt còi báo động & quạt hút thủ công).
2. **Gauge / Value Display Widgets**:
   - Gán với **V6** (Đồng hồ hiển thị Nhiệt độ).
   - Gán với **V7** (Đồng hồ hiển thị Độ ẩm).
   - Gán với **V4** (Thanh đo Nồng độ Gas).
3. **LED / State Indicator Widgets**:
   - Gán với **V5** (Đèn đỏ cảnh báo Cháy).
   - Gán với **V8** (Đèn xanh báo Trời mưa / Trạng thái giàn phơi).
4. **Text Input Widget**:
   - Gán với **V10** (Nhập tin nhắn hiển thị lên OLED từ xa).

---

## 4. Thiết Lập Tự Động Hóa & Cảnh Báo (Events & Automations)

Thiết lập trong mục **Events & Notifications** trên Blynk Web Console:
- **Sự cố rò rỉ khí gas**:
  - *Condition*: `Gas_Level (V4) > 1000`
  - *Action*: Gửi thông báo đẩy (Push Notification) đến điện thoại và tự động kích hoạt còi báo.
- **Sự cố phát hiện đám cháy**:
  - *Condition*: `Flame_Status (V5) == 1`
  - *Action*: Gửi cảnh báo khẩn cấp (Urgent Alert) tức thời đến người dùng.

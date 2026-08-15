# 🤝 Contributing to ESP32 Smart Home IoT System

Cảm ơn bạn đã quan tâm và muốn đóng góp phát triển dự án **ESP32 Smart Home IoT System**! Dưới đây là hướng dẫn quy trình tham gia đóng góp:

---

## 🛠️ Quy Trình Đóng Góp (Contribution Workflow)

1. **Fork Repository**: Nhấn nút **Fork** ở góc phải trên cùng của trang GitHub để tạo bản sao dự án vào tài khoản của bạn.
2. **Clone về máy**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/ESP32-SmartHome-IoT-System.git
   cd ESP32-SmartHome-IoT-System
   ```
3. **Tạo nhánh tính năng mới (Branching)**:
   ```bash
   git checkout -b feature/ten-tinh-nang-moi
   # Hoặc nếu là sửa lỗi:
   git checkout -b fix/ten-loi-can-sua
   ```
4. **Viết mã nguồn & Kiểm thử**:
   - Đảm bảo mã nguồn tuân theo chuẩn đặt tên biến rõ ràng.
   - Thêm chú thích giải thích logic điều khiển.
   - Kiểm tra khả năng biên dịch thành công trên Arduino IDE hoặc PlatformIO.
5. **Commit thay đổi**:
   ```bash
   git commit -m "feat: Thêm tính năng điều khiển quạt thông minh theo nhiệt độ"
   ```
6. **Push lên GitHub**:
   ```bash
   git push origin feature/ten-tinh-nang-moi
   ```
7. **Tạo Pull Request (PR)**: Mở PR trên repository gốc kèm theo mô tả chi tiết về tính năng hoặc lỗi đã được xử lý.

---

## 💡 Ý Tưởng Đóng Góp Mở Rộng
- [ ] Tích hợp giao thức MQTT kết nối với **Home Assistant** hoặc **Node-RED**.
- [ ] Bổ sung nhận diện khuôn mặt trực tiếp trên **ESP32-CAM** (Face Recognition Access).
- [ ] Tích hợp cảm biến chất lượng không khí bụi mịn (PMS5003 / SDS011).
- [ ] Thêm điều khiển bằng giọng nói qua Google Assistant / Amazon Alexa.

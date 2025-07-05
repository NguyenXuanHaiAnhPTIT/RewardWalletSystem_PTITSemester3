# 📄 README - Reward Wallet System

***

## 🎉 Giới thiệu dự án
**Hệ thống đăng nhập và đăng ký tài khoản và quản lý ví điểm thưởng** (Reward Wallet System) là ứng dụng quản lý tài khoản, ví điểm, giao dịch và quyền quản trị viên được viết bằng C++. Hệ thống hỗ trợ bảo mật mật khẩu bằng SHA-256, xác thực OTP và chức năng sao lưu, phục hồi dữ liệu.

***

## 📋 Thành viên tham gia và phân công
| Thành viên                                | Phân công công việc                                                                                                     | 
|-------------------------------------------|----------------------------------------------------------------------|
|:busts_in_silhouette: Nguyễn Minh Tuấn     |Thiết kế, xây dựng giao diện menu và quản lý luồng nghiệp vụ hệ thống. | 
|:busts_in_silhouette:Nguyễn Xuân Hải Anh   |Thiết lập, quản lý Git repository, cấu hình branch, merge code, xây dựng, quản lý tính bảo mật OTP và mã hóa mật khẩu.  | 
|:busts_in_silhouette: Phan Phúc Thịnh      |Thiết kế, phát triển và quản lý dữ liệu người dùng, ví, giao dịch. | 
|:busts_in_silhouette: Nguyễn Xuân Thanh    |Thiết kế chức năng sao lưu/khôi phục dữ liệu, thực hiện kiểm thử toàn diện, tối ưu hóa hiệu suất. | 
```bash
https://github.com/NguyenXuanHaiAnhPTIT/RewardWalletSystem_PTITSemester3/commits/main/
```

***

## 📂 Cấu trúc thư mục và mã nguồn

/RewardWalletSystem

│
├── include/  
│-----├── UserAccount.h  
│-----├── Wallet.h  
│-----├── Transaction.h  
│-----├── SystemManager.h  
│-----├── OTPManager.h  
│-----└── FileManager.h  
│
├── sha256/  
│-----├── Sha256.h  
│-----└── Sha256.cpp  
│
├── src/  
│-----├── UserAccount.cpp  
│-----├── Wallet.cpp  
│-----├── Transaction.cpp  
│-----├── SystemManager.cpp  
│-----├── OTPManager.cpp  
│-----├── FileManager.cpp  
│-----└── main.cpp  
│
├── tests/  
│-----└── ManualTestCases.md  
│
└── README.md



## 🔎 Phân tích và đặc tả chức năng
### 📌 1. Đăng ký tài khoản
- Input: Username, số điện thoại, tên, email, mật khẩu hoặc tự sinh.
- Output: Thông báo thành công và tạo ví 0 điểm.
  
### 📌 2. Đăng nhập
- Input: Username, mật khẩu.
- Output: Thông báo thành công hoặc lỗi, hiển thị menu tương ứng.

### 📌 3. Quên mật khẩu
- Input: Username, số điện thoại, OTP, mật khẩu mới.
- Output: Thông báo đặt lại thành công.

### 📌 4. Đổi mật khẩu
- Input: Mật khẩu cũ, mật khẩu mới, OTP.
- Output: Thông báo đổi thành công.

### 📌 5. Chuyển điểm
- Input: Username người nhận, số điểm, OTP.
- Output: Thông báo thành công, cập nhật số dư.
### 📌 6. Nạp điểm (Admin)
- Input: Username, số điểm, OTP.
- Output: Thông báo thành công, cập nhật số dư.

### 📌 7. Đổi mật khẩu hộ (Admin)
- Input: Username, mật khẩu mới hoặc tự sinh.
- Output: Thông báo đổi thành công.

### 📌 8. Backup / Restore
- Input: Xác nhận thao tác.
- Output: Thông báo thành công hoặc lỗi.

### 📌 9. Xem lịch sử giao dịch
- Input: Không.
- Output: Danh sách chi tiết giao dịch.

##  📥 Cách tải, biên dịch và chạy chương trình
### 📌 1. Source code
Clone hoặc tải về từ Github:

```bash
https://github.com/NguyenXuanHaiAnhPTIT/RewardWalletSystem_PTITSemester3
```

### 📌 2. Biên dịch và chạy chương trình
####  🖥️ **Visual Studio 2022**

---

## 📜 Hướng dẫn sử dụng và các chức năng

 ### 🌐 Main Menu

| Chức năng                  | Nội dung chức năng                                      | 
|----------------------------|---------------------------------------------------------|
| Dang nhap                  | Đăng nhập (gồm admin và user)                           | 
| Dang ky                    | Đăng ký tài khoản mới                                   | 
| Thoat                      | Thoát chương trình                                      | 
### 🔑 User Menu
| Chức năng                  | Nội dung chức năng                                      | 
|----------------------------|---------------------------------------------------------|
| Thông tin ca nhan          | Xem thông tin cá nhân                                   | 
| Doi mat khau               | Đổi mật khẩu (Yêu cầu OTP)                              | 
| Cap nhat thong tin ca nhan |  Cập nhật, thay đổi thông tin cá nhân (Yêu cầu OTP)     | 
| Lich su giao dich          | Xem lịch sử giao dịch                                   | 
| Dang xuat                  | Đăng xuất                                               | 
 ### 🛠️ Admin Menu
| Chức năng                  | Nội dung chức năng                                      | 
|----------------------------|---------------------------------------------------------|
| Danh sach nguoi dung       | Danh sách người dùng                                    | 
| Dang ky                    | Đổi mật khẩu (Yêu cầu OTP)                              | 
| Khoa tai khoan             |  Cập nhật, thay đổi thông tin cá nhân (Yêu cầu OTP)     | 
| Mo khoa tai khoan          | Xem lịch sử giao dịch                                   | 
| Nap diem                   | Đăng xuất                                               | 
| Lich su giao dich          | Xem thông tin cá nhân                                   | 
| Sao luu du lieu            | Đổi mật khẩu (Yêu cầu OTP)                              | 
| Khoi phuc du lieu          |  Cập nhật, thay đổi thông tin cá nhân (Yêu cầu OTP)     | 
| Dang xuat                  | Đăng xuất                                               | 


## 📚 Tài liệu tham khảo
```bash
 https://nvlpubs.nist.gov
```
```bash
 https://en.cppreference.com
```
```bash
 https://chatgpt.com
```
```bash
 https://grok.com
```
## 📝 Chú thích mã nguồn
- Code đầy đủ comment từng hàm.
- Mô tả input/output chi tiết.
- Có xử lý lỗi, rollback, xác minh OTP.

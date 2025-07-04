
## 📄 README - Reward Wallet System

###  Giới thiệu dự án
"Hệ thống quản lý ví điểm thưởng" (Reward Wallet System) là ứng dụng quản lý tài khoản, ví điểm, giao dịch và quyền quản trị viên bằng ngôn ngữ C++. Hệ thống hỗ trợ bảo mật mật khẩu bằng SHA-256, xác thực OTP và chức năng backup/restore dữ liệu.

### Thành viên tham gia và phân công
- Phan Phúc Thịnh: Module UserAccount, Wallet, Transaction; bảo mật mật khẩu, logic giao dịch.
- Nguyễn Minh Tuấn: Module SystemManager, menu, OTP, quản lý file, xử lý luồng chính.
- Nguyễn Xuân Hải Anh: Module SHA-256, kiểm thử bảo mật, OTP.
- Nguyễn Xuân Thanh: Backup/restore, kiểm tra tích hợp toàn hệ thống.
- Ngoài ra, mọi người hỗ trợ nhau viết readme và sữa lỗi hệ thống.
### Cấu trúc kho mã nguồn

/RewardWalletSystem
│
├── include/  
│   ├── UserAccount.h  
│   ├── Wallet.h  
│   ├── Transaction.h  
│   ├── SystemManager.h  
│   ├── OTPManager.h  
│   └── FileManager.h  
│
├── sha256/  
│   ├── Sha256.h  
│   └── Sha256.cpp  
│
├── src/  
│   ├── UserAccount.cpp  
│   ├── Wallet.cpp  
│   ├── Transaction.cpp  
│   ├── SystemManager.cpp  
│   ├── OTPManager.cpp  
│   ├── FileManager.cpp  
│   └── main.cpp  
│
├── tests/  
│   └── ManualTestCases.md  
│
└── README.md



###  Bản phân tích và đặc tả chức năng
#### Đăng ký tài khoản
- Input: Username, số điện thoại, tên, email, mật khẩu hoặc tự sinh.
- Output: Thông báo thành công và tạo ví 0 điểm.

#### Đăng nhập
- Input: Username, mật khẩu.
- Output: Thông báo thành công hoặc lỗi, hiển thị menu tương ứng.

#### Quên mật khẩu
- Input: Username, số điện thoại, OTP, mật khẩu mới.
- Output: Thông báo đặt lại thành công.

#### Đổi mật khẩu
- Input: Mật khẩu cũ, mật khẩu mới, OTP.
- Output: Thông báo đổi thành công.

#### Chuyển điểm
- Input: Username người nhận, số điểm, OTP.
- Output: Thông báo thành công, cập nhật số dư.

#### Nạp điểm (Admin)
- Input: Username, số điểm, OTP.
- Output: Thông báo thành công, cập nhật số dư.

#### Đổi mật khẩu hộ (Admin)
- Input: Username, mật khẩu mới hoặc tự sinh.
- Output: Thông báo đổi thành công.

#### Backup / Restore
- Input: Xác nhận thao tác.
- Output: Thông báo thành công hoặc lỗi.

#### Xem lịch sử giao dịch
- Input: Không.
- Output: Danh sách chi tiết giao dịch.

###  Cách tải và dịch chương trình
-  tải mã nguồn.
    Link github:https://github.com/NguyenXuanHaiAnhPTIT/RewardWalletSystem_PTITSemester3
- Mở trong IDE (Visual Studio,..) hoặc build với C++17 trở lên.
	Cài đặt visual studio 2022 (nếu chưa có)
         Hướng dẫn cài đặt visual stuido :
            Tải xuống và cài đặt visual studio  :https://visualstudio.microsoft.com/, sau khi cài đặt vào Visual Studio Installer--->chọn Modify---->Chọn desktop development with c++ -->Chọn install while dowloading.
        Cấu hình c++17 tại visual studio( Nêu máy tính chưa nhận):
            Sau khi mở Project:
                1. Chuột phải vào project -> Chọn Properties.
                2. Tại Configuration Properties-> General-> c++ Language Standard -->Iso c++ 17 Standard (/std:c++17)->Apply-> OK    
- Build file Main.cpp và chạy.
- Không yêu cầu thư viện ngoài.

###  Các tập tin và thư viện kèm
- Sha256.h / Sha256.cpp: Hash mật khẩu(Dùng để xác minh chứ không giải mã mật khẩu được).
- OTPManager: OTP nội bộ.
- FileManager: Lưu file user, ví, log.
- Transaction: Giao dịch.
- Wallet*: Ví điểm.
- UserAccount: Thông tin user.
- SystemManager: Luồng tổng thể.

###  Mô tả cách chạy và thao tác
1. Đăng ký hoặc admin tạo hộ.
2. Đăng nhập.
3. Thực hiện nạp điểm, chuyển điểm, đổi mật khẩu.
4. Xem lịch sử giao dịch.
5. Backup hoặc restore.

### Tài liệu tham khảo
- SHA-256 (NIST): https://nvlpubs.nist.gov
- Regex email: https://en.cppreference.com

###  Chú thích mã nguồn
- Code đầy đủ comment từng hàm.
- Mô tả input/output chi tiết.
- Có xử lý lỗi, rollback, xác minh OTP.

###  Hoàn thiện
README.md đã đầy đủ, đáp ứng yêu cầu đề bài. Sẵn sàng nộp báo cáo và demo.

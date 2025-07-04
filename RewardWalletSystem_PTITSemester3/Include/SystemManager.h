#pragma once
#include <string>
#include <functional>
#include "UserAccount.h"
#include "Wallet.h"
#include "Transaction.h"
#include "FileManager.h"
#include "OTPManager.h"

// Quản lý nghiệp vụ tổng thể, giao diện, luồng chương trình
class SystemManager {
private:
    UserAccount* currentUser;
    Wallet* currentWallet;
    FileManager fileManager;
    OTPManager otpManager;
public:
    SystemManager();
    void mainMenu();               // Giao diện chính
    void registerUser();
    void registerUserAsAdmin();
    void login();
    void forgotPassword();                                 // Quên mật khẩu qua menu
    void forgotPassword(const std::string& username);      // Quên mật khẩu khi đang nhập username
    std::string generateRandomPassword(const std::string& username);
    void logout();
    void changePassword();
    void updatePersonalInfo();
    void transferPoints();
    void topupUserWallet();        // Admin nạp điểm cho user
    void lockUser();
    void unlockUser();
    void backup();
    void restore();
    void viewTransactionLog();
    void viewUserList();
    void adminMenu();
    void userMenu();
    void resetUserPasswordByAdmin();
};

// Hàm hỗ trợ nhập liệu an toàn
bool safeInput(const std::string& prompt, std::function<bool(const std::string&)> validator, std::string& out, const std::string& errMsg);
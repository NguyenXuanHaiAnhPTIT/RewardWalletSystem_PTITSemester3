#include "../include/SystemManager.h"
#include "../sha256/Sha256.h"
#include <iostream>
#include <random>
#include <chrono>
#include <regex>
using namespace std;


// Hàm hỗ trợ nhập liệu lặp lại đúng quy tắc, hỏi xác nhận nếu sai 3 lần
bool safeInput(const std::string& prompt, std::function<bool(const std::string&)> validator, std::string& out, const std::string& errMsg) {
    int fail = 0;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, out);
        if (validator(out)) return true;
        std::cout << errMsg << "\n";
        ++fail;
        if (fail >= 3) {
            std::string opt;
            std::cout << "Ban da nhap sai qua 3 lan. Ban co muon tiep tuc nhap truong nay? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") return false;
            fail = 0;
        }
    }
}
//Hàm kiểm tra email hợp lệ
bool isValidEmail(const std::string& email) {
    static const std::regex pattern(
        R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))"
    );
    return std::regex_match(email, pattern);
}
bool validateEmailInput(const std::string& s) {
    if (s.empty()) return true;
    if (s.length() > 50) return false;
    return isValidEmail(s);
}

// --------- Constructor ---------
SystemManager::SystemManager() {
    currentUser = nullptr;
    currentWallet = nullptr;
}

// --------- Main Menu ---------
void SystemManager::mainMenu() {
    while (true) {
        std::cout << "-----------------------------------\n";
        std::cout << "  REWARD WALLET SYSTEM - PTIT SEM3\n";
        std::cout << "-----------------------------------\n";
        std::cout << "1. Dang nhap\n";
        std::cout << "2. Dang ky tai khoan\n";
        std::cout << "3. Thoat\n";
        std::string choice;
        bool ok = safeInput("Chon chuc nang (1-3): ",
            [](const std::string& s) { return s == "1" || s == "2" || s == "3"; },
            choice, "Lua chon khong hop le."
        );
        if (!ok) return;
        if (choice == "1") login();
        else if (choice == "2") registerUser();
        else if (choice == "3") {
            std::cout << "Tam biet!\n";
            break;
        }
    }
}

// --------- Register User ---------
void SystemManager::registerUser() {
    UserAccount user;
    std::cout << "\n--- DANG KY TAI KHOAN ---\n";

    // Username
    bool ok = safeInput(
        "Username (*): ",
        [&](const std::string& s) {
            if (s.empty() || s.length() > 50) return false;
            UserAccount tmp;
            return !fileManager.loadUser(s, tmp);
        },
        user.username,
        "Username khong duoc de trong, vuot 50 ky tu hoac da ton tai."
    );
    if (!ok) { std::cout << "Huy tac vu dang ky.\n"; return; }

    // Số điện thoại
    ok = safeInput(
        "So dien thoai (*): ",
        [](const std::string& s) {
            if (s.empty() || s.length() < 9 || s.length() > 12) return false;
            if (s[0] == '+') {
                for (size_t i = 1; i < s.length(); ++i)
                    if (!isdigit(s[i])) return false;
            }
            else {
                for (char c : s)
                    if (!isdigit(c)) return false;
            }
            return true;
        }
        ,
        user.phone,
        "So dien thoai chi co do  dai tu 9 den 12 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu dang ky.\n"; return; }

    // Họ tên (có thể bỏ trống)
    ok = safeInput(
        "Ho ten (bo trong neu khong co): ",
        [](const std::string& s) { return s.empty() || s.length() <= 50; },
        user.fullName,
        "Ho ten khong duoc vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu dang ky.\n"; return; }

    // Email (có thể bỏ trống, nếu nhập thì phải hợp lệ)
    ok = safeInput(
        "Email (bo trong neu khong co): ",
        validateEmailInput,
        user.email, // hoặc newEmail
        "Email khong dung dinh dang hoac vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu dang ky.\n"; return; }

    // Password
    std::string password, confirm;
    int fail = 0;
    while (true) {
        std::cout << "Nhap mat khau (bo trong de dung mat khau tu sinh): ";
        std::getline(std::cin, password);
        if (!password.empty()) {
            if (password.length() > 50) {
                std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                ++fail;
            }
            else {
                bool hasLower = false, hasDigit = false, hasSpecial = false;
                for (char ch : password) {
                    if (std::islower(ch)) hasLower = true;
                    else if (std::isdigit(ch)) hasDigit = true;
                    else if (!std::isalnum(ch)) hasSpecial = true;
                }
                if (!(hasLower && hasDigit && hasSpecial)) {
                    std::cout << "Mat khau phai co it nhat 1 chu thuong, 1 chu so va 1 ky tu dac biet.\n";
                    ++fail;
                }
                else {
                    int confirmFail = 0;
                    while (true) {
                        std::cout << "Nhap lai mat khau: ";
                        std::getline(std::cin, confirm);
                        if (confirm.length() > 50) {
                            std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                            ++confirmFail;
                        }
                        else if (confirm != password) {
                            std::cout << "Mat khau khong trung khop.\n";
                            ++confirmFail;
                        }
                        else {
                            user.isPasswordAutoGenerated = false;
                            goto passok;
                        }
                        if (confirmFail >= 3) {
                            std::string opt;
                            std::cout << "Nhap lai mat khau sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
                            std::getline(std::cin, opt);
                            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu dang ky.\n"; return; }
                            confirmFail = 0;
                        }
                    }
                }
            }
        }
        else {
            password = generateRandomPassword(user.username);
            std::cout << "Mat khau tu sinh: " << password << "\n";
            user.isPasswordAutoGenerated = true;
            goto passok;
        }
        if (fail >= 3) {
            std::string opt;
            std::cout << "Nhap mat khau sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu dang ky.\n"; return; }
            fail = 0;
        }
    }
passok:
    user.hashedPassword = Sha256(password);
    user.isAdmin = false;
    user.isLocked = false;
    if (fileManager.saveUser(user)) {
        Wallet w(user.username + "_wallet", user.username, 0);
        fileManager.saveWallet(w);
        std::cout << "Dang ky thanh cong! Vui long dang nhap.\n";
    }
    else std::cout << "Loi khi luu tai khoan!\n";
}

// --------- Đăng ký tài khoản hộ (Admin) ---------
void SystemManager::registerUserAsAdmin() {
    UserAccount user;
    std::cout << "\n--- ADMIN TAO TAI KHOAN HO NGUOI DUNG ---\n";
    // Username
    bool ok = safeInput(
        "Username (*): ",
        [&](const std::string& s) {
            if (s.empty() || s.length() > 50) return false;
            UserAccount tmp;
            return !fileManager.loadUser(s, tmp);
        },
        user.username,
        "Username khong duoc de trong, vuot 50 ky tu hoac da ton tai."
    );
    if (!ok) { std::cout << "Huy tac vu tao tai khoan.\n"; return; }
    // Số điện thoại
    ok = safeInput(
        "So dien thoai (*): ",
        [](const std::string& s) {
            if (s.empty() || s.length() < 9 || s.length() > 12) return false;
            for (char c : s) if (!isdigit(c)) return false;
            return true;
        },
        user.phone,
        "So dien thoai chi duoc chua so va co do dai tu 9 den 12 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu tao tai khoan.\n"; return; }
    // Họ tên (có thể bỏ trống)
    ok = safeInput(
        "Ho ten (bo trong neu khong co): ",
        [](const std::string& s) { return s.empty() || s.length() <= 50; },
        user.fullName,
        "Ho ten khong duoc vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu tao tai khoan.\n"; return; }
    // Email (có thể bỏ trống, nếu nhập thì phải hợp lệ)
    ok = safeInput(
        "Email (bo trong neu khong co): ",
        validateEmailInput,
        user.email, // hoặc newEmail
        "Email khong dung dinh dang hoac vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu dang ky.\n"; return; }
    // Password
    std::string password, confirm;
    int fail = 0;
    while (true) {
        std::cout << "Nhap mat khau (bo trong de dung mat khau tu sinh): ";
        std::getline(std::cin, password);
        if (!password.empty()) {
            if (password.length() > 50) {
                std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                ++fail;
            }
            else {
                bool hasLower = false, hasDigit = false, hasSpecial = false;
                for (char ch : password) {
                    if (std::islower(ch)) hasLower = true;
                    else if (std::isdigit(ch)) hasDigit = true;
                    else if (!std::isalnum(ch)) hasSpecial = true;
                }
                if (!(hasLower && hasDigit && hasSpecial)) {
                    std::cout << "Mat khau phai co it nhat 1 chu thuong, 1 chu so va 1 ky tu dac biet.\n";
                    ++fail;
                }
                else {
                    int confirmFail = 0;
                    while (true) {
                        std::cout << "Nhap lai mat khau: ";
                        std::getline(std::cin, confirm);
                        if (confirm.length() > 50) {
                            std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                            ++confirmFail;
                        }
                        else if (confirm != password) {
                            std::cout << "Mat khau khong trung khop.\n";
                            ++confirmFail;
                        }
                        else {
                            user.isPasswordAutoGenerated = false;
                            goto passok2;
                        }
                        if (confirmFail >= 3) {
                            std::string opt;
                            std::cout << "Nhap lai mat khau sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
                            std::getline(std::cin, opt);
                            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu tao tai khoan.\n"; return; }
                            confirmFail = 0;
                        }
                    }
                }
            }
        }
        else {
            password = generateRandomPassword(user.username);
            std::cout << "Mat khau tu sinh: " << password << "\n";
            user.isPasswordAutoGenerated = true;
            goto passok2;
        }
        if (fail >= 3) {
            std::string opt;
            std::cout << "Nhap mat khau sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu tao tai khoan.\n"; return; }
            fail = 0;
        }
    }
passok2:
    user.hashedPassword = Sha256(password);
    user.isAdmin = false;
    user.isLocked = false;
    if (fileManager.saveUser(user)) {
        Wallet w(user.username + "_wallet", user.username, 0);
        fileManager.saveWallet(w);
        std::cout << "Tao tai khoan thanh cong!\n";
    }
    else std::cout << "Loi khi luu tai khoan!\n";
}

// --------- Đăng nhập ---------
void SystemManager::login() {
    std::cout << "\n--- DANG NHAP ---\n";
    std::string username, password;
    bool ok = safeInput(
        "Username: ",
        [](const std::string& s) { return !s.empty(); },
        username,
        "Username khong duoc de trong."
    );
    if (!ok) { std::cout << "Huy tac vu dang nhap.\n"; return; }
    UserAccount user;
    if (!fileManager.loadUser(username, user)) {
        std::cout << "Username khong ton tai!\n";
        return;
    }
    int fail = 0;
    while (true) {
        std::cout << "Mat khau (hoac nhap 'Q' de quen mat khau): ";
        std::getline(std::cin, password);
        if (password == "Q" || password == "q") {
            forgotPassword(username);
            return;
        }
        if (user.hashedPassword == Sha256(password)) break;
        std::cout << "Mat khau sai!\n";
        ++fail;
        if (fail >= 3) {
            std::string opt;
            std::cout << "Ban co muon quen mat khau? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt == "Y" || opt == "y") { forgotPassword(username); return; }
            else return;
        }
    }
    if (user.isLocked) {
        std::cout << "Tai khoan bi khoa! Lien he admin.\n";
        return;
    }
    currentUser = new UserAccount(user);
    Wallet w;
    fileManager.loadWallet(currentUser->username, w);
    currentWallet = new Wallet(w);
    std::cout << "Dang nhap thanh cong!\n";
    if (currentUser->isPasswordAutoGenerated) {
        std::cout << "Ban dang su dung mat khau tu sinh, vui long doi mat khau moi!\n";
        changePassword();
    }
    if (currentUser->isAdmin) adminMenu();
    else userMenu();
    delete currentUser; delete currentWallet;
    currentUser = nullptr; currentWallet = nullptr;
}

// --------- Quên mật khẩu (biết username) ---------
void SystemManager::forgotPassword(const std::string& username) {
    UserAccount user;
    if (!fileManager.loadUser(username, user)) {
        std::cout << "Username khong ton tai!\n";
        return;
    }
    std::cout << "\n--- QUEN MAT KHAU ---\n";
    std::string phone;
    bool ok = safeInput(
        "Nhap so dien thoai da dang ky: ",
        [&](const std::string& s) { return !s.empty() && s == user.phone; },
        phone,
        "So dien thoai khong dung hoac de trong."
    );
    if (!ok) { std::cout << "Huy tac vu quen mat khau.\n"; return; }
    if (!otpManager.requestOTPWithTimeout()) {
        std::cout << "Da huy tac vu vi xac thuc OTP that bai hoac het han.\n";
        return;
    }
    // Nhập lại mật khẩu mới
    std::string newPass, confirm;
    int fail = 0;
    while (true) {
        std::cout << "Nhap mat khau moi (bo trong de dung mat khau tu sinh): ";
        std::getline(std::cin, newPass);
        if (!newPass.empty()) {
            if (newPass.length() > 50) {
                std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                ++fail;
            }
            else {
                bool hasLower = false, hasDigit = false, hasSpecial = false;
                for (char ch : newPass) {
                    if (std::islower(ch)) hasLower = true;
                    else if (std::isdigit(ch)) hasDigit = true;
                    else if (!std::isalnum(ch)) hasSpecial = true;
                }
                if (!(hasLower && hasDigit && hasSpecial)) {
                    std::cout << "Mat khau phai co it nhat 1 chu thuong, 1 chu so va 1 ky tu dac biet.\n";
                    ++fail;
                }
                else {
                    int confirmFail = 0;
                    while (true) {
                        std::cout << "Nhap lai mat khau moi: ";
                        std::getline(std::cin, confirm);
                        if (confirm.length() > 50) {
                            std::cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                            ++confirmFail;
                        }
                        else if (confirm != newPass) {
                            std::cout << "Mat khau moi khong trung khop.\n";
                            ++confirmFail;
                        }
                        else {
                            user.isPasswordAutoGenerated = false;
                            goto passok3;
                        }
                        if (confirmFail >= 3) {
                            std::string opt;
                            std::cout << "Nhap lai mat khau moi sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
                            std::getline(std::cin, opt);
                            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu quen mat khau.\n"; return; }
                            confirmFail = 0;
                        }
                    }
                }
            }
        }
        else {
            newPass = generateRandomPassword(user.username);
            std::cout << "Mat khau tu sinh: " << newPass << "\n";
            user.isPasswordAutoGenerated = true;
            goto passok3;
        }
        if (fail >= 3) {
            std::string opt;
            std::cout << "Nhap mat khau moi sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu quen mat khau.\n"; return; }
            fail = 0;
        }
    }
passok3:
    user.hashedPassword = Sha256(newPass);
    if (fileManager.saveUser(user)) {
        std::cout << "Dat lai mat khau thanh cong! Vui long dang nhap lai.\n";
    }
    else {
        std::cout << "Loi khi dat lai mat khau!\n";
    }
}

// --------- Quên mật khẩu (menu nhập username) ---------
void SystemManager::forgotPassword() {
    std::string username;
    bool ok = safeInput(
        "\n--- QUEN MAT KHAU ---\nNhap username: ",
        [](const std::string& s) { return !s.empty(); },
        username,
        "Username khong duoc de trong."
    );
    if (!ok) { std::cout << "Huy tac vu quen mat khau.\n"; return; }
    forgotPassword(username);
}
// --- Sinh mật khẩu tự động ---
std::string SystemManager::generateRandomPassword(const std::string& username) {
    static const char specialChars[] = "!@#$%^&*";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> specialDist(0, sizeof(specialChars) - 2);
    std::uniform_int_distribution<> digitDist(0, 9);

    std::string password = username;
    password += specialChars[specialDist(rng)];
    for (int i = 0; i < 3; ++i) {
        password += std::to_string(digitDist(rng));
    }
    return password;
}

// --- Đăng xuất ---
void SystemManager::logout() {
    std::cout << "Dang xuat thanh cong!\n";
}

// --- Đổi mật khẩu ---
void SystemManager::changePassword() {
    std::cout << "\n--- DOI MAT KHAU ---\n";
    std::string oldPass, newPass, confirmPass;
    int fail = 0;
    while (true) {
        std::cout << "Nhap mat khau cu: ";
        std::getline(std::cin, oldPass);
        if (currentUser->checkPassword(oldPass)) break;
        std::cout << "Mat khau cu sai!\n";
        ++fail;
        if (fail >= 3) {
            std::string opt;
            std::cout << "Nhap mat khau cu sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu doi mat khau.\n"; return; }
            fail = 0;
        }
    }
    // Nhập mật khẩu mới
    fail = 0;
    while (true) {
        std::cout << "Nhap mat khau moi: ";
        std::getline(std::cin, newPass);
        if (newPass.length() > 50) {
            std::cout << "Mat khau moi khong duoc vuot qua 50 ky tu!\n";
            ++fail;
        }
        else {
            bool hasLower = false, hasDigit = false, hasSpecial = false;
            for (char ch : newPass) {
                if (std::islower(ch)) hasLower = true;
                else if (std::isdigit(ch)) hasDigit = true;
                else if (!std::isalnum(ch)) hasSpecial = true;
            }
            if (!(hasLower && hasDigit && hasSpecial)) {
                std::cout << "Mat khau moi phai co it nhat 1 chu thuong, 1 chu so va 1 ky tu dac biet.\n";
                ++fail;
            }
            else {
                int confirmFail = 0;
                while (true) {
                    std::cout << "Xac nhan mat khau moi: ";
                    std::getline(std::cin, confirmPass);
                    if (confirmPass.length() > 50) {
                        std::cout << "Mat khau moi khong duoc vuot qua 50 ky tu!\n";
                        ++confirmFail;
                    }
                    else if (confirmPass != newPass) {
                        std::cout << "Mat khau moi khong trung khop!\n";
                        ++confirmFail;
                    }
                    else goto passok4;
                    if (confirmFail >= 3) {
                        std::string opt;
                        std::cout << "Nhap xac nhan sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
                        std::getline(std::cin, opt);
                        if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu doi mat khau.\n"; return; }
                        confirmFail = 0;
                    }
                }
            }
        }
        if (fail >= 3) {
            std::string opt;
            std::cout << "Nhap mat khau moi sai qua 3 lan. Ban co muon tiep tuc nhap khong? (Y/N): ";
            std::getline(std::cin, opt);
            if (opt != "Y" && opt != "y") { std::cout << "Huy tac vu doi mat khau.\n"; return; }
            fail = 0;
        }
    }
passok4:
    if (!otpManager.requestOTPWithTimeout()) {
        std::cout << "Da huy tac vu vi xac thuc OTP that bai hoac het han.\n";
        return;
    }
    currentUser->hashedPassword = Sha256(newPass);
    currentUser->isPasswordAutoGenerated = false;
    if (fileManager.saveUser(*currentUser)) {
        std::cout << "Doi mat khau thanh cong!\n";
    }
    else {
        std::cout << "Loi khi luu mat khau moi!\n";
    }
}

// --- Cập nhật thông tin cá nhân ---
void SystemManager::updatePersonalInfo() {
    std::cout << "\n--- CAP NHAT THONG TIN CA NHAN ---\n";
    std::string newName, newEmail, newPhone;

    // Họ tên
    bool ok = safeInput(
        "Nhap ho ten moi (bo trong neu khong muon doi): ",
        [](const std::string& s) { return s.empty() || s.length() <= 50; },
        newName,
        "Ho ten khong duoc vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu cap nhat thong tin.\n"; return; }

    // Email
    ok = safeInput(
        "Nhap email moi (bo trong neu khong muon doi): ",
        validateEmailInput,
        newEmail,
        "Email khong dung dinh dang hoac vuot qua 50 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu cap nhat thong tin.\n"; return; }


    // Số điện thoại
    ok = safeInput(
        "Nhap so dien thoai moi (bo trong neu khong muon doi): ",
        [](const std::string& s) {
            if (s.empty()) return true;
            if (s.length() < 9 || s.length() > 12) return false;
            for (char c : s) if (!isdigit(c)) return false;
            return true;
        },
        newPhone,
        "So dien thoai chi duoc chua so va co do dai tu 9 den 12 ky tu!"
    );
    if (!ok) { std::cout << "Huy tac vu cap nhat thong tin.\n"; return; }

    // Không thay đổi gì thì báo và return
    if (newName.empty() && newEmail.empty() && newPhone.empty()) {
        std::cout << "Khong co thong tin nao can cap nhat.\n";
        return;
    }
    // Xác thực OTP
    if (!otpManager.requestOTPWithTimeout()) {
        std::cout << "Da huy tac vu vi xac thuc OTP that bai hoac het han.\n";
        return;
    }
    if (!newName.empty()) currentUser->fullName = newName;
    if (!newEmail.empty()) currentUser->email = newEmail;
    if (!newPhone.empty()) currentUser->phone = newPhone;

    if (fileManager.saveUser(*currentUser)) {
        std::cout << "Cap nhat thong tin thanh cong!\n";
    }
    else {
        std::cout << "Loi khi cap nhat thong tin!\n";
    }
}


// --- Chuyển điểm ---
void SystemManager::transferPoints() {
    std::cout << "\n--- CHUYEN DIEM ---\n";
    std::string toUsername;
    bool ok = safeInput(
        "Nhap username nguoi nhan: ",
        [&](const std::string& s) {
            if (s.empty()) return false;
            if (s == currentUser->username) return false;
            UserAccount u;
            return fileManager.loadUser(s, u) && !u.isLocked;
        },
        toUsername,
        "Nguoi nhan khong ton tai, bi khoa hoac ban dang tu chuyen cho chinh minh!"
    );
    if (!ok) { std::cout << "Huy tac vu chuyen diem.\n"; return; }

    std::string inputAmount;
    ok = safeInput(
        "Nhap so diem can chuyen: ",
        [](const std::string& s) {
            if (s.empty()) return false;
            for (char c : s) if (!isdigit(c)) return false;
            long long a = std::stoll(s);
            return a > 0;
        },
        inputAmount,
        "So diem chuyen phai la so nguyen duong!"
    );
    if (!ok) { std::cout << "Huy tac vu chuyen diem.\n"; return; }
    long long amount = std::stoll(inputAmount);

    Wallet fromWallet, toWallet;
    if (!fileManager.loadWallet(currentUser->username, fromWallet) ||
        !fileManager.loadWallet(toUsername, toWallet)) {
        std::cout << "Loi doc du lieu vi!\n";
        return;
    }
    if (fromWallet.balance < amount) {
        std::cout << "So du khong du! Giao dich bi huy.\n";
        return;
    }
    if (!otpManager.requestOTPWithTimeout()) {
        std::cout << "Da huy tac vu vi xac thuc OTP that bai hoac het han.\n";
        return;
    }
    fromWallet.balance -= amount;
    toWallet.balance += amount;
    bool ok1 = fileManager.saveWallet(fromWallet);
    bool ok2 = fileManager.saveWallet(toWallet);

    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now_c);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    std::string timeStamp = buf;

    Transaction tx(
        "TX-" + currentUser->username + "-" + toUsername + "-" + std::to_string(std::time(nullptr)),
        currentUser->username,
        toUsername,
        amount,
        timeStamp,
        (ok1 && ok2) ? "Success" : "Failed",
        (ok1 && ok2) ? "Chuyen diem thanh cong" : "Loi khi ghi du lieu vi"
    );
    fileManager.saveTransaction(tx);

    if (ok1 && ok2) {
        std::cout << "Chuyen diem thanh cong!\n";
        std::cout << "So du hien tai: " << fromWallet.balance << " diem\n";
    }
    else {
        fromWallet.balance += amount;
        toWallet.balance -= amount;
        fileManager.saveWallet(fromWallet);
        fileManager.saveWallet(toWallet);
        std::cout << "Co loi khi luu giao dich. So du da duoc phuc hoi.\n";
    }
}

// --- Nạp điểm cho user (admin) ---
void SystemManager::topupUserWallet() {
    std::cout << "\n--- ADMIN NAP DIEM CHO USER ---\n";
    std::string username;
    bool ok = safeInput(
        "Nhap username can nap diem: ",
        [&](const std::string& s) {
            if (s.empty()) return false;
            UserAccount u;
            return fileManager.loadUser(s, u) && !u.isLocked;
        },
        username,
        "Nguoi dung khong ton tai hoac dang bi khoa!"
    );
    if (!ok) { std::cout << "Huy tac vu nap diem.\n"; return; }

    std::string inputAmount;
    ok = safeInput(
        "Nhap so diem can nap: ",
        [](const std::string& s) {
            if (s.empty()) return false;
            for (char c : s) if (!isdigit(c)) return false;
            long long a = std::stoll(s);
            return a > 0;
        },
        inputAmount,
        "So diem nap phai la so nguyen duong!"
    );
    if (!ok) { std::cout << "Huy tac vu nap diem.\n"; return; }
    long long amount = std::stoll(inputAmount);

    if (!otpManager.requestOTPWithTimeout()) {
        std::cout << "Da huy tac vu vi xac thuc OTP that bai hoac het han.\n";
        return;
    }

    Wallet userWallet;
    if (!fileManager.loadWallet(username, userWallet)) {
        std::cout << "Khong the nap du lieu vi nguoi nhan.\n";
        return;
    }
    userWallet.balance += amount;
    bool ok2 = fileManager.saveWallet(userWallet);

    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now_c);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    std::string timeStamp = buf;

    Transaction tx(
        "TX-ADMIN-TOPUP-" + username + "-" + std::to_string(std::time(nullptr)),
        "ADMIN", username, amount, timeStamp,
        ok2 ? "Success" : "Failed",
        ok2 ? "Nap diem thanh cong" : "Loi khi ghi du lieu vi"
    );
    fileManager.saveTransaction(tx);

    if (ok2) {
        std::cout << "Nap diem thanh cong! So du moi: " << userWallet.balance << " diem\n";
    }
    else {
        std::cout << "Loi khi nap diem!\n";
    }
}

// --- Khóa tài khoản ---
void SystemManager::lockUser() {
    std::cout << "\n--- ADMIN KHOA TAI KHOAN USER ---\n";
    std::string username;
    bool ok = safeInput(
        "Nhap username can khoa: ",
        [&](const std::string& s) {
            if (s.empty()) return false;
            UserAccount u;
            if (!fileManager.loadUser(s, u)) return false;
            return !u.isAdmin && !u.isLocked;
        },
        username,
        "User khong ton tai, da khoa hoac la admin!"
    );
    if (!ok) { std::cout << "Huy tac vu khoa user.\n"; return; }
    UserAccount user;
    fileManager.loadUser(username, user);
    user.isLocked = true;
    if (fileManager.saveUser(user)) {
        std::cout << "Khoa tai khoan thanh cong!\n";
    }
    else {
        std::cout << "Loi khi khoa tai khoan!\n";
    }
}

// --- Mở khóa tài khoản ---
void SystemManager::unlockUser() {
    std::cout << "\n--- ADMIN MO KHOA TAI KHOAN USER ---\n";
    std::string username;
    bool ok = safeInput(
        "Nhap username can mo khoa: ",
        [&](const std::string& s) {
            if (s.empty()) return false;
            UserAccount u;
            if (!fileManager.loadUser(s, u)) return false;
            return !u.isAdmin && u.isLocked;
        },
        username,
        "User khong bi khoa hoac khong ton tai!"
    );
    if (!ok) { std::cout << "Huy tac vu mo khoa user.\n"; return; }
    UserAccount user;
    fileManager.loadUser(username, user);
    user.isLocked = false;
    if (fileManager.saveUser(user)) {
        std::cout << "Mo khoa tai khoan thanh cong!\n";
    }
    else {
        std::cout << "Loi khi mo khoa tai khoan!\n";
    }
}

// --- Backup dữ liệu ---
void SystemManager::backup() {
    std::cout << "\n--- BACKUP DU LIEU ---\n";
    bool ok = fileManager.backupData();
    if (ok) std::cout << "Backup du lieu thanh cong!\n";
    else std::cout << "Loi khi backup du lieu!\n";
}

// --- Restore dữ liệu ---
void SystemManager::restore() {
    std::cout << "\n--- PHUC HOI DU LIEU TU BACKUP ---\n";
    std::string xacnhan;
    bool ok = safeInput(
        "Canh bao: Phuc hoi se ghi de toan bo du lieu hien tai! Ban co chac muon tiep tuc? (Y/N): ",
        [](const std::string& s) { return s == "Y" || s == "y" || s == "N" || s == "n"; },
        xacnhan,
        "Chi duoc nhap Y/y hoac N/n."
    );
    if (!ok || xacnhan == "N" || xacnhan == "n") {
        std::cout << "Huy thao tac phuc hoi.\n";
        return;
    }
    bool result = fileManager.restoreBackup();
    if (result) std::cout << "Phuc hoi du lieu thanh cong!\n";
    else std::cout << "Loi khi phuc hoi du lieu!\n";
}

// --- Xem lịch sử giao dịch ---
void SystemManager::viewTransactionLog() {
    std::cout << "\n--- LICH SU GIAO DICH ---\n";
    auto txs = fileManager.loadAllTransactions();
    if (txs.empty()) {
        std::cout << "Khong co giao dich nao!\n";
        return;
    }
    for (const auto& t : txs) {
        if (currentUser->isAdmin ||
            t.fromUser == currentUser->username ||
            t.toUser == currentUser->username) {
            t.display();
            std::cout << "-------------------------\n";
        }
    }
}

// --- Xem danh sách người dùng (admin) ---
void SystemManager::viewUserList() {
    std::cout << "\n--- DANH SACH NGUOI DUNG ---\n";
    auto users = fileManager.loadAllUsers();
    if (users.empty()) {
        std::cout << "Khong co nguoi dung nao!\n";
        return;
    }
    for (const auto& u : users) {
        Wallet w;
        fileManager.loadWallet(u.username, w);
        std::cout << "==============================\n";
        std::cout << "Username: " << u.username << "\n";
        std::cout << "Ho ten : " << u.fullName << "\n";
        std::cout << "Trang thai: " << (u.isLocked ? "Bi khoa" : "Binh thuong") << "\n";
        std::cout << "Vai tro   : " << (u.isAdmin ? "Admin" : "User") << "\n";
        std::cout << "Vi: " << w.walletID << " - So du: " << w.balance << "\n";
        std::cout << "==============================\n";
    }
}

// --- Menu User ---
void SystemManager::userMenu() {
    while (true) {
        std::cout << "\n--- MENU NGUOI DUNG ---\n";
        std::cout << "1. Xem thong tin ca nhan\n";
        std::cout << "2. Doi mat khau\n";
        std::cout << "3. Chuyen diem\n";
        std::cout << "4. Cap nhat thong tin ca nhan\n";
        std::cout << "5. Xem lich su giao dich\n";
        std::cout << "6. Dang xuat\n";
        std::string choice;
        bool ok = safeInput("Chon chuc nang: ",
            [](const std::string& s) { return s == "1" || s == "2" || s == "3" || s == "4" || s == "5" || s == "6"; },
            choice, "Lua chon khong hop le.");
        if (!ok) return;
        if (choice == "1") currentUser->displayInfo();
        else if (choice == "2") changePassword();
        else if (choice == "3") transferPoints();
        else if (choice == "4") updatePersonalInfo();
        else if (choice == "5") viewTransactionLog();
        else if (choice == "6") break;
    }
}

// --- Menu Admin ---
void SystemManager::adminMenu() {
    while (true) {
        std::cout << "1. Xem danh sach nguoi dung\n";
        std::cout << "2. Dang ki tai khoan ho nguoi dung\n";
        std::cout << "3. Khoa tai khoan nguoi dung\n";
        std::cout << "4. Doi mat khau ho nguoi dung\n";     
        std::cout << "5. Mo khoa tai khoan nguoi dung\n"; 
        std::cout << "6. Nap diem cho user\n";
        std::cout << "7. Xem lich su giao dich\n";
        std::cout << "8. Backup du lieu\n";
        std::cout << "9. Restore du lieu\n";
        std::cout << "10. Dang xuat\n";

        std::string choice;
        bool ok = safeInput("Chon chuc nang: ",
            [](const std::string& s) {
                return (s >= "1" && s <= "9" && s.length() == 1) || s == "10"; },
                choice, "Lua chon khong hop le.");
        if (!ok) return;
        if (choice == "1") viewUserList();
        else if (choice == "2") registerUserAsAdmin();
        else if (choice == "3") lockUser();
        else if (choice == "4") resetUserPasswordByAdmin();
        else if (choice == "5") unlockUser();
        else if (choice == "6") topupUserWallet();
        else if (choice == "7") viewTransactionLog();
        else if (choice == "8") backup();
        else if (choice == "9") restore();
        else if (choice == "10") break;
    }
}


void SystemManager::resetUserPasswordByAdmin() {
    cout << "\n--- ADMIN DOI MAT KHAU HO NGUOI DUNG ---\n";
    string username;

    bool ok = safeInput(
        "Nhap username can doi mat khau: ",
        [&](const string& s) {
            if (s.empty()) return false;
            UserAccount u;
            return fileManager.loadUser(s, u) && !u.isLocked && !u.isAdmin;
        },
        username,
        "User khong ton tai, da khoa hoac la admin!"
    );

    if (!ok) {
        cout << "Huy tac vu doi mat khau.\n";
        return;
    }

    UserAccount user;
    fileManager.loadUser(username, user);

    string newPass, confirm;
    int fail = 0;
    while (true) {
        cout << "Nhap mat khau moi (bo trong de dung mat khau tu sinh): ";
        getline(cin, newPass);

        if (!newPass.empty()) {
            if (newPass.length() > 50) {
                cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                ++fail;
            }
            else {
                bool hasLower = false, hasDigit = false, hasSpecial = false;
                for (char ch : newPass) {
                    if (islower(ch)) hasLower = true;
                    else if (isdigit(ch)) hasDigit = true;
                    else if (!isalnum(ch)) hasSpecial = true;
                }
                if (!(hasLower && hasDigit && hasSpecial)) {
                    cout << "Mat khau phai co it nhat 1 chu thuong, 1 chu so va 1 ky tu dac biet.\n";
                    ++fail;
                }
                else {
                    int confirmFail = 0;
                    while (true) {
                        cout << "Nhap lai mat khau moi: ";
                        getline(cin, confirm);
                        if (confirm.length() > 50) {
                            cout << "Mat khau khong duoc vuot qua 50 ky tu!\n";
                            ++confirmFail;
                        }
                        else if (confirm != newPass) {
                            cout << "Mat khau moi khong trung khop!\n";
                            ++confirmFail;
                        }
                        else {
                            user.isPasswordAutoGenerated = false;
                            goto passok;
                        }
                        if (confirmFail >= 3) {
                            string opt;
                            cout << "Nhap lai sai qua 3 lan. Ban co muon tiep tuc? (Y/N): ";
                            getline(cin, opt);
                            if (opt != "Y" && opt != "y") {
                                cout << "Huy tac vu doi mat khau.\n";
                                return;
                            }
                            confirmFail = 0;
                        }
                    }
                }
            }
        }
        else {
            newPass = generateRandomPassword(user.username);
            cout << "Mat khau tu sinh: " << newPass << "\n";
            user.isPasswordAutoGenerated = true;
            goto passok;
        }

        if (fail >= 3) {
            string opt;
            cout << "Nhap mat khau moi sai qua 3 lan. Ban co muon tiep tuc? (Y/N): ";
            getline(cin, opt);
            if (opt != "Y" && opt != "y") {
                cout << "Huy tac vu doi mat khau.\n";
                return;
            }
            fail = 0;
        }
    }

passok:
    user.hashedPassword = Sha256(newPass);

    if (fileManager.saveUser(user)) {
        cout << "Doi mat khau thanh cong!\n";
    }
    else {
        cout << "Loi khi luu mat khau moi!\n";
    }
}

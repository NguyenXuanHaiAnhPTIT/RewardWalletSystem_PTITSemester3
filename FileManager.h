#pragma once
#include <string>
#include <vector>
#include "UserAccount.h"
#include "Wallet.h"
#include "Transaction.h"

// Đọc/ghi dữ liệu file, backup, restore, tạo folder nếu chưa có
class FileManager {
public:
    FileManager();
    // Quản lý User
    bool saveUser(const UserAccount& user);
    bool loadUser(const std::string& username, UserAccount& user);
    std::vector<UserAccount> loadAllUsers();
    // Quản lý Wallet
    bool saveWallet(const Wallet& wallet);
    bool loadWallet(const std::string& username, Wallet& wallet);
    // Quản lý Transaction
    bool saveTransaction(const Transaction& t);
    std::vector<Transaction> loadAllTransactions();
    // Backup & restore
    bool backupData();
    bool restoreBackup();
    // Tạo folder nếu chưa có
    static void createFolderIfNotExists(const std::string& path);
};
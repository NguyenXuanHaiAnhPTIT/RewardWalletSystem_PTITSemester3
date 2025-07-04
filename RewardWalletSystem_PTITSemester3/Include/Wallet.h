#pragma once
#include <string>
using namespace std;

// Đại diện cho ví điểm thưởng của user hoặc admin
class Wallet {
public:
    string walletID;    // Hash từ username hoặc 1 mã duy nhất
    string owner;       // username sở hữu ví
    long long balance;       // Số điểm hiện tại

    Wallet();
    Wallet(const string& walletID, const string& owner, long long balance = 0);

    void display() const;
};
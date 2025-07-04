#include "../include/Wallet.h"
#include <iostream>
using namespace std;

// Constructor mặc định
Wallet::Wallet()
    : walletID(""), owner(""), balance(0)
{
}

// Constructor đầy đủ
Wallet::Wallet(const string& walletID, const string& owner, long long balance)
    : walletID(walletID), owner(owner), balance(balance)
{
}

void Wallet::display() const {
    cout << "----- Wallet Info -----" << endl;
    cout << "Wallet ID : " << walletID << endl;
    cout << "Owner     : " << owner << endl;
    cout << "Balance   : " << balance << endl;
}
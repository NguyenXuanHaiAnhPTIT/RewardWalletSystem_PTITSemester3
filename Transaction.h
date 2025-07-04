#pragma once
#include <string>
using namespace std;

// Giao dịch chuyển điểm giữa các ví
class Transaction {
public:
    string transactionID;   // Sinh bằng timestamp + random
    string fromUser;
    string toUser;
    long long amount;
    string timeStamp;       // Lưu dưới dạng string
    string status;          // "Success" / "Failed"
    string note;            // Lý do thất bại...

    Transaction();
    Transaction(const string& transactionID,
        const string& fromUser,
        const string& toUser,
        long long amount,
        const string& timeStamp,
        const string& status,
        const string& note);

    void display() const;
};
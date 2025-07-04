#include "../include/Transaction.h"
#include <iostream>
using namespace std;

// Constructor mặc định
Transaction::Transaction() :
    transactionID(""),
    fromUser(""),
    toUser(""),
    amount(0),
    timeStamp(""),
    status(""),
    note("")
{
}

// Constructor đầy đủ
Transaction::Transaction(const string& transactionID,
    const string& fromUser,
    const string& toUser,
    long long amount,
    const string& timeStamp,
    const string& status,
    const string& note)
    : transactionID(transactionID),
    fromUser(fromUser),
    toUser(toUser),
    amount(amount),
    timeStamp(timeStamp),
    status(status),
    note(note)
{
}

// Hiển thị thông tin giao dịch ra console
void Transaction::display() const {
    cout << "Transaction ID : " << transactionID << endl;
    cout << "From User      : " << fromUser << endl;
    cout << "To User        : " << toUser << endl;
    cout << "Amount         : " << amount << endl;
    cout << "Time           : " << timeStamp << endl;
    cout << "Status         : " << status << endl;
    cout << "Note           : " << note << endl;
}

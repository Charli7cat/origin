#include <iostream>
#include <string>

struct BankAccount {
    int accountNumber;
    std::string ownerName;
    double balance;
};

void changeBalance(BankAccount& account, double newBalance) {
    account.balance = newBalance;
}

void printAccount(const BankAccount& account) {
    std::cout << "Your account: "
        << account.ownerName << ", "
        << account.accountNumber << ", "
        << account.balance << std::endl;
}

int main() {
    BankAccount account;

    std::cout << "=== BANK ACCOUNT ===" << std::endl;

    std::cout << "Enter the account number: ";
    std::cin >> account.accountNumber;

    std::cout << "Enter the name of the owner: ";
    std::cin >> account.ownerName;  

    std::cout << "Enter the balance: ";
    std::cin >> account.balance;

    std::cout << "\nCurrent account information:" << std::endl;
    printAccount(account);

    double newBalance;
    std::cout << "\nEnter a new balance: ";
    std::cin >> newBalance;

    changeBalance(account, newBalance);

    std::cout << "\nUpdated account information:" << std::endl;
    printAccount(account);

    EXIT_SUCCESS;

}
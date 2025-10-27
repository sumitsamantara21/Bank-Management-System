#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <limits>
using namespace std;

// Utility function to get current date string
string currentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    return string(buf);
}

// Transaction record for bank account
struct Transaction {
    string type;      // Deposit, Withdrawal, Transfer In, Transfer Out
    double amount;
    string date;
};

///////////////////////////////
// Employee Classes
///////////////////////////////

class Employee {
protected:
    int empId;
    string empName;
public:
    virtual void inputInfo() = 0;
    virtual void calculateSalary() const = 0;
    void inputBasicInfo() {
        cout << "Enter Employee ID: "; cin >> empId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Employee Name: "; getline(cin, empName);
    }
    void displayInfo() const {
        cout << "Employee ID: " << empId << "\nName: " << empName << endl;
    }
    virtual ~Employee() {}
};

class VisitingEmployee : public Employee {
    int workedDays;
    float dailyWage;
public:
    void inputInfo() override {
        cout << "Enter daily wage: "; cin >> dailyWage;
        cout << "Enter number of worked days: "; cin >> workedDays;
    }
    void calculateSalary() const override {
        cout << "Salary: Rs " << workedDays * dailyWage << endl;
    }
};

class PermanentEmployee : public Employee {
    int monthlySalary;
public:
    void inputInfo() override {
        cout << "Enter monthly salary: "; cin >> monthlySalary;
    }
    void calculateSalary() const override {
        cout << "Monthly Salary: Rs " << monthlySalary << endl;
    }
};

///////////////////////////////
// Bank Account Classes
///////////////////////////////

class BankAccount {
    string name;
    int accountNum;
    double balance;
    vector<Transaction> transactions;
public:
    BankAccount(string n, int ac, double bal) : name(n), accountNum(ac), balance(bal) {}

    string getName() const { // made const
        return name;
    }
    int getAccountNum() const { // made const
        return accountNum;
    }
    double getBalance() const { // made const
        return balance;
    }
    void deposit(double amount) {
        balance += amount;
        transactions.push_back({"Deposit", amount, currentDate()});
        cout << "\t\tDeposit successful.\n";
    }
    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            transactions.push_back({"Withdrawal", amount, currentDate()});
            cout << "\t\tWithdraw successful.\n";
        } else {
            cout << "\t\tInsufficient Balance.\n";
        }
    }
    bool transferTo(BankAccount &receiver, double amount){
        if(balance >= amount){
            balance -= amount;
            receiver.balance += amount;
            transactions.push_back({"Transfer Out", amount, currentDate()});
            receiver.transactions.push_back({"Transfer In", amount, currentDate()});
            cout << "\t\tTransfer successful.\n";
            return true;
        }
        else {
            cout << "\t\tInsufficient Balance for Transfer.\n";
            return false;
        }
    }

    void printTransactionHistory(int lastN = 5) const {
        cout << "\tLast " << lastN << " transactions for Account " << accountNum << ":\n";
        int start = (transactions.size() >= lastN) ? transactions.size() - lastN : 0;
        for (size_t i = start; i < transactions.size(); i++) {
            const Transaction &t = transactions[i];
            cout << "\t" << t.date << " - " << t.type << ": Rs " << t.amount << "\n";
        }
        if (transactions.empty()) cout << "\tNo transactions yet.\n";
    }

    void display() const { // made const
        cout << "\tName: " << name << ", Account Number: " << accountNum << ", Balance: Rs " << balance << endl;
    }
};

class BankManagement {
    vector<BankAccount> accounts;
public:
    void AddAccount(string name, int accountNum, double balance) {
        for (auto& a : accounts) {
            if (a.getAccountNum() == accountNum) {
                cout << "\t\tAccount number already exists!\n";
                return;
            }
        }
        accounts.push_back(BankAccount(name, accountNum, balance));
        cout << "\t\tAccount Created Successfully.\n";
    }
    void showAllAccounts() {
        cout << "\t\tAll Account Holders:\n";
        if(accounts.empty()){ cout<<"\t\tNo accounts found.\n"; return;}
        for (auto& acc : accounts) {
            acc.display();
        }
    }
    void searchAccount(int account) {
        cout << "\t\tSearch Account:\n";
        bool found = false;
        for (auto& acc : accounts) {
            if (acc.getAccountNum() == account) {
                acc.display();
                found = true;
            }
        }
        if(!found) cout<<"\t\tAccount not found.\n";
    }
    BankAccount* findAccount(int accountNum) {
        for (auto& acc : accounts) {
            if (acc.getAccountNum() == accountNum)
                return &acc;
        }
        return nullptr;
    }
};

///////////////////////////////
// Currency Converter Class
///////////////////////////////

class CurrencyConvertor {
    double usdToEuroRate, euroToUsdRate, usdToIndRate, indToEuroRate;
public:
    CurrencyConvertor(double usdEuro, double euroUsd, double usdInd, double indEuro)
        : usdToEuroRate(usdEuro), euroToUsdRate(euroUsd),
          usdToIndRate(usdInd), indToEuroRate(indEuro) {}
    double usdToEuro(double amt) const { return amt * usdToEuroRate; }
    double euroToUsd(double amt) const { return amt * euroToUsdRate; }
    double usdToInd(double amt) const { return amt * usdToIndRate; }
    double indToEuro(double amt) const { return amt * indToEuroRate; }
    void displayRates() const {
        cout << "\nCurrent Conversion Rates:\n";
        cout << "1 USD = " << usdToEuroRate << " Euro\n";
        cout << "1 Euro = " << euroToUsdRate << " USD\n";
        cout << "1 USD = Rs " << usdToIndRate << "\n";
        cout << "1 Rs = " << indToEuroRate << " Euro\n";
    }
};

///////////////////////////////
// ATM Operations
///////////////////////////////

void atmMenu(BankManagement& bank) {
    int acNum, choice;
    double amount;
    cout << "Enter your Account Number: "; cin >> acNum;
    BankAccount* acc = bank.findAccount(acNum);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }
    do {
        cout << "\n--- ATM Menu ---\n"
             << "1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Transaction History\n6. Exit\nChoose option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Balance: Rs " << acc->getBalance() << endl;
                break;
            case 2:
                cout << "Deposit Amount (Rs): "; cin >> amount;
                acc->deposit(amount);
                break;
            case 3:
                cout << "Withdraw Amount (Rs): "; cin >> amount;
                acc->withdraw(amount);
                break;
            case 4: {
                cout << "Enter Recipient Account Number: ";
                int recipient;
                cin >> recipient;
                BankAccount* receiver = bank.findAccount(recipient);
                if (!receiver) {
                    cout << "Recipient account not found.\n";
                    break;
                }
                cout << "Enter Transfer Amount (Rs): "; cin >> amount;
                if (!acc->transferTo(*receiver, amount))
                    cout << "Insufficient balance.\n";
                break;
            }
            case 5:
                acc->printTransactionHistory(10);
                break;
            case 6:
                cout << "Exiting ATM...\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != 6);
}

///////////////////////////////
// Main - Integration of All Modules
///////////////////////////////

int main() {
    BankManagement bank;
    CurrencyConvertor convertor(0.85, 1.18, 85.0, 0.0099);

    int choice;
    do {
        cout << "\n====== Banking System Management ======\n";
        cout << "1. Employee Management\n2. Bank Account Management\n3. Currency Converter\n4. ATM Services\n5. Exit\nChoose option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\n-- Employee Management --\n";
                int empChoice;
                cout << "1. Visiting Employee\n2. Permanent Employee\nChoose: ";
                cin >> empChoice;
                Employee* emp = nullptr;
                if (empChoice == 1) emp = new VisitingEmployee();
                else if (empChoice == 2) emp = new PermanentEmployee();
                else {
                    cout << "Invalid choice.\n";
                    break;
                }
                if(emp){
                    emp->inputBasicInfo();
                    emp->inputInfo();
                    cout << "\nEmployee Details:\n";
                    emp->displayInfo();
                    emp->calculateSalary();
                    delete emp;
                }
                break;
            }
            case 2: {
                cout << "\n-- Bank Account Management --\n";
                int bChoice;
                cout << "1. Create New Account\n2. Show All Accounts\n3. Search Account\nChoose: ";
                cin >> bChoice;
                switch(bChoice) {
                    case 1: {
                        string name; int acNum; double bal;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter Name: "; getline(cin, name);
                        cout << "Enter Account Number: "; cin >> acNum;
                        cout << "Enter Initial Balance (Rs): "; cin >> bal;
                        bank.AddAccount(name, acNum, bal);
                        break;
                    }
                    case 2:
                        bank.showAllAccounts();
                        break;
                    case 3: {
                        int accNum;
                        cout << "Enter Account Number to Search: "; cin >> accNum;
                        bank.searchAccount(accNum);
                        break;
                    }
                    default:
                        cout << "Invalid choice.\n";
                }
                break;
            }
            case 3: {
                cout << "\n-- Currency Converter --\n";
                int cChoice;
                double amt;
                convertor.displayRates();
                cout << "1. USD to Euro\n2. Euro to USD\n3. USD to Rs\n4. Rs to Euro\nChoose conversion: ";
                cin >> cChoice;
                cout << "Enter Amount: "; cin >> amt;
                switch (cChoice) {
                    case 1: cout << amt << " USD = " << convertor.usdToEuro(amt) << " Euro\n"; break;
                    case 2: cout << amt << " Euro = " << convertor.euroToUsd(amt) << " USD\n"; break;
                    case 3: cout << amt << " USD = Rs " << convertor.usdToInd(amt) << "\n"; break;
                    case 4: cout << amt << " Rs = " << convertor.indToEuro(amt) << " Euro\n"; break;
                    default: cout << "Invalid choice.\n"; break;
                }
                break;
            }
            case 4:
                atmMenu(bank);
                break;
            case 5:
                cout << "Thank you for using the banking system. Goodbye!\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
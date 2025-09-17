#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;


struct Date {
    int day, month, year;
};


string getTypeTransaction(int type) {
    if (type == 1) return "deposit";
    else if (type == 2) return "withdrawal";
    else if (type == 3) return "transfer";
    else return "unknow! Please choose again";
}

Date getToday() {               //supported by chatGPT to manager date easily
    time_t now = time(0);
    tm *ltm = localtime(&now);

    Date today;
    today.day = ltm->tm_mday;
    today.month = 1 + ltm->tm_mon;
    today.year = 1900 + ltm->tm_year;

    return today;
}


class Transaction {
    private:
        int amount;
        int type;
        Date date;
    public:
        Transaction(int amount, int type, Date date) {
            this->amount = amount;
            this->type = type;
            this->date = date;
        }


        //getter and setter
        int getAmount() {return amount;}
        string getType() {return getTypeTransaction(type);}
        Date getDate() {return date;}


        void setAmount(int amount) {this->amount = amount;}
        void setType(int type) {this->type = type;}
        void setDate(Date date) {this->date = date;}


        void displayInfo() {
            cout << "The " + getTypeTransaction(type) + " transaction with " << amount
                        << " VND on " << date.day << "/" << date.month << "/" << date.year << endl;
        }
};


class Account {
    private:
    protected:
        string accountNumber;
        double balance;
        string ownerName;
        vector<Transaction> transactionHistory;
    public:
        Account(string accountNumber, string ownerName, double balance) {
            this->accountNumber = accountNumber;
            this->ownerName = ownerName;
            this->balance = balance;
        }


        //getter and setter
        string getAccountNumber() {return accountNumber;}
        string getOwnerName() {return ownerName;}
        double getBalance() {return balance;}


        void setAccountNumber(string accountNumber) {this->accountNumber = accountNumber;}
        void setOwnerName(string ownerName) {this->ownerName = ownerName;}
        void setBalance(double balance) {this->balance = balance;}


        void deposit(double amount) {
            Date today = getToday();
            balance += amount;
            cout << "You have successfully deposited money!" << endl;
            cout << "The balance after deposit: " << balance << " VND" << endl;
            transactionHistory.push_back(Transaction(amount, 1, today));
        }

        virtual void withdrawal(double amount, double fee=0, double limit=0) {
            Date today = getToday();
            if (balance >= amount) {
                balance -= amount;
                cout << "You have successfully withdrawn!" << endl;
                cout << "The balance after withdrawal: " << balance << " VND" << endl;
                transactionHistory.push_back(Transaction(amount, 2, today));
            }
            else cout << "The balance is not enough!" << endl;
        }

        // operator overloading
        void operator+=(Transaction t) {
            transactionHistory.push_back(t);
        }

        bool operator==(const Account &other) {
            return this->balance == other.balance;
        }

        void displayInfo() {
            cout << "Account Number: " << accountNumber << endl;
            cout << "Owner: " << ownerName << endl;
            cout << "Balance: " << balance << " VND" << endl;
            for (auto &t : transactionHistory) {
                t.displayInfo();
            }
        }

        void balanceInquire() {
            cout << "The current balance of account " << accountNumber << " is: " << balance << " VND" << endl;
        }
};


class SavingAccount : public Account {
    private:
        double interestRate; // 1-20(%)
    public:
        SavingAccount(string accountNumber, string ownerName, double balance, double interestRate) : Account(accountNumber, ownerName,balance) {
            this->interestRate = interestRate;
        }


        //getter and setter
        double getInterestRate() {return interestRate;}
        void setInterestRate(double interestRate) {this->interestRate = interestRate;}


        void applyInterest(int month) {
            for (int i=1; i<=month; i++){
                balance = balance*(1 + interestRate/100);
            }
            cout << "After " << month << " month, your balance is " << balance << " VND" << endl;
        }

        void withdrawal(double amount, double fee, double limit) override {
            if (balance - (amount + fee) >= limit) {
                Date today = getToday();
                balance = balance - (amount + fee);
                cout << "You have successfully withdrawn!" << endl;
                cout << "The balance after withdrawal: " << balance << " VND" << endl;
                transactionHistory.push_back(Transaction(amount, 2, today));
            }
            else cout << "The balance is not enough!" << endl;
        }
};

class Customer {
    private:
        string name;
        string ID;
        vector<Account*> accounts;
    public:
        Customer(string name, string ID) {
            this->name = name;
            this->ID = ID;
        }

        //getter and setter
        string getName() {return name;}
        string getID() {return ID;}

        void setName(string name) {this->name = name;}
        void setID(string ID) {this->ID = ID;}

        void openNewAccount(Account *acc) {
            accounts.push_back(acc);
        }

        double getTotalBalance() {
            double total = 0;
            for (auto acc : accounts) {
                total += acc->getBalance();
            }
            return total;
        }

        void displayAccounts() {
            cout << "Customer: " << name << " (ID: " << ID << ")" << endl;
            for (auto acc : accounts) {
                acc->displayInfo();
                cout << endl;
            }
        }
};

int main() {
    // Tạo khách hàng
    Customer c1("Nguyen Van A", "123456789");

    // Tạo tài khoản thường và tiết kiệm
    Account *acc1 = new Account("AC001", "Nguyen Van A", 5000);
    SavingAccount *acc2 = new SavingAccount("AC002", "Nguyen Van A", 10000, 5);

    // Gắn vào khách hàng
    c1.openNewAccount(acc1);
    c1.openNewAccount(acc2);

    // In thông tin có sẵn
    cout << "===== THONG TIN KHOI TAO =====" << endl;;
    cout << "Khach hang: " << c1.getName() << " (ID: " << c1.getID() << ")" << endl;;
    cout << "Tai khoan hien co:" << endl;;
    cout << "- Account thuong: " << acc1->getAccountNumber()
         << ", So du: " << acc1->getBalance() << " VND" << endl;;
    cout << "- SavingAccount: " << acc2->getAccountNumber()
         << ", So du: " << acc2->getBalance()
         << " VND, Lai suat: " << acc2->getInterestRate() << "%" << endl;;

    int choice;
    do {
        cout << "===== MENU NGAN HANG =====" << endl;;
        cout << "1. Nap tien" << endl;;
        cout << "2. Rut tien" << endl;;
        cout << "3. Xem so du" << endl;;
        cout << "4. Xem lich su giao dich" << endl;;
        cout << "5. Ap dung lai suat (SavingAccount)" << endl;;
        cout << "6. Tong so du khach hang" << endl;;
        cout << "7. In tat ca thong tin khach hang" << endl;;
        cout << "0. Thoat" << endl;;
        cout << "Nhap lua chon: ";
        cin >> choice;

        if (choice == 1) {
            string accNum;
            double amount;
            cout << "Nhap so tai khoan: ";
            cin >> accNum;
            cout << "Nhap so tien muon nap: ";
            cin >> amount;
            if (accNum == acc1->getAccountNumber()) acc1->deposit(amount);
            else if (accNum == acc2->getAccountNumber()) acc2->deposit(amount);
            else cout << "Khong tim thay tai khoan!" << endl;;
        }
        else if (choice == 2) {
            string accNum;
            double amount;
            cout << "Nhap so tai khoan: ";
            cin >> accNum;
            cout << "Nhap so tien muon rut: ";
            cin >> amount;
            if (accNum == acc1->getAccountNu mber()) acc1->withdrawal(amount);
            else if (accNum == acc2->getAccountNumber()) {
                double fee = 100, limit = 2000;
                acc2->withdrawal(amount, fee, limit);
            }
            else cout << "Khong tim thay tai khoan!" << endl;;
        }
        else if (choice == 3) {
            string accNum;
            cout << "Nhap so tai khoan: ";
            cin >> accNum;
            if (accNum == acc1->getAccountNumber()) acc1->isBalanceInquire();
            else if (accNum == acc2->getAccountNumber()) acc2->isBalanceInquire();
            else cout << "Khong tim thay tai khoan!" << endl;;
        }
        else if (choice == 4) {
            c1.displayAccounts();
        }
        else if (choice == 5) {
            int month;
            cout << "Nhap so thang muon tinh lai: ";
            cin >> month;
            acc2->applyInterest(month);
        }
        else if (choice == 6) {
            cout << "Tong so du khach hang: " << c1.getTotalBalance() << " VND" << endl;;
        }
        else if (choice == 7) {
            cout << "===== THONG TIN KHACH HANG =====" << endl;;
            c1.displayAccounts();
        }
        else if (choice == 0) {
            cout << "Thoat chuong trinh..." << endl;;
        }
        else {
            cout << "Lua chon khong hop le!" << endl;;
        }

    } while (choice != 0);

    delete acc1;
    delete acc2;
    return 0;
}


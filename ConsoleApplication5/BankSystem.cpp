#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include<exception>

using namespace std;

ifstream Inp;
ofstream Out; // Gobal

class User
{
protected:
    string username;
    string password;

public:
    User()
    {
    }
    User(string username, string password)
    {
        this->username = username;
        this->password = password;
    }

    virtual bool login() = 0;
    void setusername(string name)
    {
        username = name;
    }
    void setpassword(string pass)
    {
        password = pass;
    }
    void changePassword()
    {
        string newPassword;
        cout << "\t\t\tEnter new password: ";
        cin >> newPassword;
        password = newPassword;
        cout << "\t\t\tPassword changed successfully!" << endl;
    }
    string getname()
    {
        return username;
    }
    string getpassword()
    {
        return password;
    }
    virtual ~User() {}
};

class Customer : public User
{
private:
public:
    Customer()
    {
    }
    Customer(string username, string password)
        : User(username, password)
    {
    }

    bool login()
    {
        string enteredUsername, enteredPassword;
        cout << "Enter the Username: ";
        cin >> enteredUsername;

        if (enteredUsername != username) {
            cout << "Invalid username. Login failed." << endl;
            return false;
        }

        cout << "Enter your password: ";
        cin >> enteredPassword;

        if (enteredPassword == password)
        {
            cout << "Login successful!" << endl;
            return true;
        }
        else
        {
            cout << "Invalid password. Login failed." << endl;
            cout << "If you want to change the password press Y" << endl;
            char y;
            cin >> y;
            if (y == 'Y' || y == 'y')
            {
                changePassword();
            }
            return false;
        }
    }
    ~Customer() {

    }
};

class Admin : public User
{

public:
    Admin()
    {
        username = "admin";
        password = "admin";
    }
    Admin(string username, string password)
        : User(username, password)
    {
    }

    void display()
    {
        cout << "Admin" << endl;
    }

    bool login()
    {
        string enter_Username, enter_Password;
        cout << "\t\Enter admin username: ";
        cin >> enter_Username;
        cout << "\t\Enter admin password: ";
        cin >> enter_Password;

        if (enter_Username == "admin" && enter_Password == "admin")
        {
            cout << "\t\t\tAdmin login successful!" << endl;
            return true;
        }
        else
        {
            cout << "\t\Invalid admin credentials. Login failed." << endl;
            cout << "\t\If you want to change the password press Y" << endl;
            char y;
            cin >> y;
            if (y == 'Y' || y == 'y')
            {
                Admin::changePassword();

            }
            return false;
        }
    }
    ~Admin() {

    }
};

class UserBuilder {
private:
    string username;
    string password;

public:
    UserBuilder() {}

    UserBuilder& setUsername(const string& name) {
        this->username = name;
        return *this;
    }

    UserBuilder& setPassword(const string& pass) {
        this->password = pass;
        return *this;
    }
};

class Account
{
protected:
    int accountNumber;
    string name;
    double balance;
    int phonenumber;

public:
    Account()
    {
        accountNumber = 0;
        name = "";
        balance = 0;
        phonenumber = 0;
    }
    Account(string name, int accountNumber, double balance, int phonenumber)
    {
        this->accountNumber = accountNumber;
        this->balance = balance;
        this->phonenumber = phonenumber;
        this->name = name;
    }

    string getName()
    {
        return name;
    }

    void setName(string Name)
    {
        name = Name;
    }

    int getphone()
    {
        return phonenumber;
    }
    void setphone(int Num)
    {
        phonenumber = Num;
    }

    int getAccountNumber()
    {
        return accountNumber;
    }
    void setBalance(double bal)
    {
        balance = bal;
    }
    double getBalance()
    {
        return balance;
    }
    void setAccountNumber(int accno)
    {
        accountNumber = accno;
    }
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void display() const = 0;
    virtual void saveAccountData() = 0;
    virtual void deleteaccount(string filename, int size) = 0;

    virtual ~Account() {}
};

class SavingAccount : public Account
{
private:
    string savedatafile;
    int size;
    Customer** customer;
    SavingAccount** saveaccount;
    Admin admin;


public:
    SavingAccount()
    {
        customer = nullptr;
        saveaccount = nullptr;
        size = 0;
    }
    SavingAccount(string filename, int S)
    {
        this->savedatafile = filename;
        this->size = S;

        saveaccount = new SavingAccount * [size];
        customer = new Customer * [size];
        for (int i = 0; i < size; i++)
        {
            saveaccount[i] = nullptr;
            customer[i] = nullptr;
        }

        Inp.open(filename);
        if (!Inp)
        {
            cout << "Failed to open Saving acount data file." << endl;
            return;
        }
        int index = 0;
        string line;
        while (getline(Inp, line))
        {

            istringstream SS(line);
            // Read data
            string fname, lname, user_nnam, pass_word, C_type, Email;
            int accountNO, phone_num;
            double bal;

            SS >> name >> user_nnam >> pass_word >> accountNO >> bal >> phone_num;

            customer[index] = new Customer(user_nnam, pass_word);
            saveaccount[index] = new SavingAccount(name, accountNO, bal, phone_num);

            index++;
            if (index >= size)
            {
                break; // to avoid buffer
            }
        }

        Inp.close();
    }

    SavingAccount(string name, int accountNumber, double balance, int phonenumber)
        : Account(name, accountNumber, balance, phonenumber)
    {
    }

    void deposit(double amount)
    {
        int accountno;
        cout << "Enter the account Number : ";
        cin >> accountno;

        bool found = false;
        int Index = -1;

        for (int i = 0; i < size; i++)
        {
            if (saveaccount[i]->accountNumber == accountno)
            {
                found = true;
                Index = i;
                break;
            }

        }
        if (found)
        {
            if (customer[Index]->login())
            {
                saveaccount[Index]->balance += amount;
                cout << "Deposit of $" << amount << " into account " << saveaccount[Index]->accountNumber << " successful." << endl;
                SavingAccount::saveAccountData();
            }

        }
        else
        {
            cout << left <<  "Account not found! " << endl;
        }
    }
    void withdraw(double amount)
    {
        int accountno;
        cout << "Enter the account Number : ";
        cin >> accountno;

        bool found = false;
        int Index = -1;

        for (int i = 0; i < size; i++)
        {
            if (saveaccount[i]->getAccountNumber() == accountno)
            {
                found = true;
                Index = i;
                break;
            }

        }
        if (found)
        {
            if (customer[Index]->login())
            {
                try
                {
                    if (saveaccount[Index]->balance < amount) {
                        throw runtime_error("Insufficient balance in account: ");
                    }
                    else
                    {
                        saveaccount[Index]->balance -= amount;
                        cout << "Withdrawal of $" << amount << " from account " << saveaccount[Index]->accountNumber << " successful." << endl;
                        SavingAccount::saveAccountData();

                    }

                }
                catch (runtime_error e)
                {
                    cout << e.what() << endl;
                    cout << "Please Enter the sufficient balance less then your account balance" << endl;
                    double Amount_W;
                    cout << "Enter the Amount you want to withdraw" << endl;
                    cin >> Amount_W;
                    SavingAccount::withdraw(Amount_W);

                }

            }

        }
        else
        {
            cout << "Account not found" << endl;
        }
    }
    void deleteaccount(string filename, int s)
    {
        int deleteacc;
        if (admin.login())
        {
            SavingAccount::display();

            cout << left << setw(30) << "Enter the account you want to delete: ";
            cin >> deleteacc;
            bool found = false;
            int deleteIndex = -1;
            for (int i = 0; i < size; i++)
            {
                if (saveaccount[i]->getAccountNumber() == deleteacc)
                {
                    cout << setw(30) << "\t\tName: " << saveaccount[i]->getName() << endl;
                    cout << setw(30) << "\t\tUsername: " << customer[i]->getname() << endl;
                    cout << setw(30) << "\t\tAccount Number: " << saveaccount[i]->accountNumber << endl;
                    cout << setw(30) << "\t\tBalance: $" << saveaccount[i]->balance << endl;
                    cout << setw(30) << "\t\tPhone Number: " << saveaccount[i]->getphone() << endl;

                    found = true;
                    deleteIndex = i;
                    break;
                }
            }
            if (found)
            {
                char a;
                cout << "\n\t\t\tIf you want to delete this account enter Y...";
                cin >> a;
                if (a == 'Y' || a == 'y')
                {


                    if (deleteIndex >= 0 && deleteIndex < size) {
                        delete customer[deleteIndex];
                        delete saveaccount[deleteIndex];
                        for (int i = deleteIndex; i < size - 1; i++)
                        {
                            customer[i] = customer[i + 1];
                            saveaccount[i] = saveaccount[i + 1];
                        }
                        size--;
                        SavingAccount::saveAccountData();
                        cout << setw(30) << "Account deleted successfully " << endl;
                    }
                }
                else {
                    cout << "Account is not deleted" << endl;
                }
            }
            else
            {
                cout << left << setw(30) << "Account not found." << endl;
            }
        }

    }

    void saveAccountData()
    {
        Out.open(savedatafile, ios::out);

        if (!Out)
        {
            cout << setw(30) << "Failed to open saving account data file." << endl;
            return;
        }
        Customer* cust;
        SavingAccount* saveaccou;
        // Write student
        for (int i = 0; i < size; i++)
        {
            cust = customer[i];
            saveaccou = saveaccount[i];
            Out << left << setw(20) << saveaccou->getName()
                << left << setw(20) << cust->getname()
                << left << setw(20) << cust->getpassword()
                << left << setw(20) << saveaccou->getAccountNumber()
                << left << setw(20) << saveaccou->getBalance()
                << left << setw(30) << saveaccou->getphone()
                << endl;
        }
        Out.close();
    }

    void display() const
    {
        cout << left << setw(30) << "Account Type: Savings" << endl;
        cout << setw(50) << "================================CUSTOMER FOR SAVING ACCOUNT LIST=====================================" << endl;
        cout << left << setw(10) << "A/C"
            << left << setw(20) << "Name"
            << left << setw(20) << "No. Account"
            << left << setw(20) << "Username"
            << left << setw(15) << "Phone No."
            << left << setw(15) << "Balance"
            << endl;

        for (int i = 0; i < size; i++)
        {
            cout << left << setw(10) << "Saving" << left << setw(20) << saveaccount[i]->getName()
                << left << setw(20) << saveaccount[i]->getAccountNumber() << left << setw(20) << customer[i]->getname() << left << setw(15)
                << left << setw(15) << saveaccount[i]->getphone() << left << setw(15) << saveaccount[i]->getBalance() << endl;
        }
    }

    ~SavingAccount()
    {
        SavingAccount::saveAccountData();
        if (saveaccount != nullptr)

        {
            delete[] saveaccount;
        }
        if (customer != nullptr) {
            delete[] customer;
        }

    }
};

class CheckAccount : public Account {
private:
    string checkdatafile;
    int size;
    Customer** customer;
    CheckAccount** checkaccount;
    Admin admin;
    double feecharge;
public:
    CheckAccount() {
        customer = nullptr;
        checkaccount = nullptr;
        size = 0;
    }

    CheckAccount(string filename, int size)
    {
        checkdatafile = filename;
        this->size = size;
        this->feecharge = 0.5;
        customer = new Customer * [size];
        checkaccount = new CheckAccount * [size];

        for (int i = 0; i < size; i++)
        {
            checkaccount[i] = nullptr;
            customer[i] = nullptr;
        }

        checkdatafile = filename;

        Inp.open(filename);
        if (!Inp)
        {
            cout << setw(30) << "Failed to open Checking class data file." << endl;
            return;
        }
        int index = 0;
        string line;
        while (getline(Inp, line))
        {
            stringstream SS(line);

            // Read data
            string name, user_nnam, pass_word;
            int accountNO, phone_num;
            double bal;

            SS >> name >> user_nnam >> pass_word >> accountNO >> bal >> phone_num;



            customer[index] = new Customer(user_nnam, pass_word);
            checkaccount[index] = new CheckAccount(name, accountNO, bal, phone_num);

            index++;
            if (index >= size)
            {

                break;
            }
        }

        Inp.close();
    }

    CheckAccount(string name, int accountNumber, double balance, int phonenumber)
        : Account(name, accountNumber, balance, phonenumber) {
        this->feecharge = 0.5;
    }

    void deposit(double amount)
    {
        cout << "\n======================================================================================\n";
        int accountno;
        cout << left << "Enter the account Number: ";
        cin >> accountno;

        bool found = false;
        int Index = -1;

        for (int i = 0; i < size; i++)
        {
            if (checkaccount[i]->accountNumber == accountno)
            {
                found = true;
                Index = i;
                break;
            }
        }
        if (found)
        {
            if (customer[Index]->login())
            {
                amount -= feecharge;
                checkaccount[Index]->balance += amount;
                cout << "Fee charges $" << feecharge << " dedicated from your Balance\n";
                cout << left << setw(30) << "Deposit of $" << amount << " into account " << checkaccount[Index]->accountNumber << " successful " << endl;
                CheckAccount::saveAccountData();
            }

        }
    }

    void withdraw(double amount)
    {

        int accountno;
        cout << left << "Enter the account Number: ";
        cin >> accountno;

        bool found = false;
        int Index = -1;

        for (int i = 0; i < size; i++)
        {
            if (checkaccount[i]->getAccountNumber() == accountno)
            {
                found = true;
                Index = i;
                break;
            }
        }
        if (found)
        {
            if (customer[Index]->login())
            {
                try
                {
                    if (checkaccount[Index]->balance >= amount)
                    {

                        checkaccount[Index]->balance -= amount;
                        checkaccount[Index]->balance -= feecharge;
                        cout << "Fee charges $" << feecharge << " dedicated from your Balance\n";
                        cout << "Withdrawal of $" << amount << " from account " << checkaccount[Index]->accountNumber << " successful " << endl;
                        CheckAccount::saveAccountData();
                    }
                    else
                    {
                        throw runtime_error("\t\t\tInsufficient balance in account ");

                    }
                }
                catch (runtime_error e)
                {
                    cout << e.what() << endl;
                    cout << "Please Enter the sufficient balance less then your account balance" << endl;
                    double Amount_W;
                    cout << "Enter the Amount you want to withdraw" << endl;
                    cin >> Amount_W;
                    CheckAccount::withdraw(Amount_W);

                }

            }

        }
        else {
            cout << "Account not found" << endl;
        }
    }

    void deleteaccount(string filename, int s)
    {
        int deleteacc;
        if (admin.login())
        {
            CheckAccount::display();
            cout << "tEnter the account you want to delete: ";
            cin >> deleteacc;
            bool found = false;
            int deleteIndex = -1;
            for (int i = 0; i < size; i++)
            {
                if (checkaccount[i]->getAccountNumber() == deleteacc)
                {
                    cout << "Account Found! " << endl;
                    cout << setw(30) << "\t\t\tName: " << checkaccount[i]->getName() << endl;
                    cout << setw(30) << "\t\t\tAccount Holder Name: " << customer[i]->getname() << endl;
                    cout << setw(30) << "\t\t\tAccount Number: " << checkaccount[i]->getAccountNumber() << endl;
                    cout << setw(30) << "\t\t\tBalance: $" << checkaccount[i]->getBalance() << endl;
                    cout << setw(30) << "\t\t\tPhone Number: " << checkaccount[i]->getphone() << endl;
                    found = true;
                    deleteIndex = i;
                    break;
                }
            }
            if (found)
            {
                char y;
                cout << "\t\Enter Y to delete this account... ";
                cin >> y;
                if (y == 'Y' || y == 'y') {


                    delete customer[deleteIndex];
                    delete checkaccount[deleteIndex];
                    for (int i = deleteIndex; i < size - 1; i++)
                    {
                        customer[i] = customer[i + 1];
                        checkaccount[i] = checkaccount[i + 1];
                    }
                    size--;
                    CheckAccount::saveAccountData();
                    cout << "Account deleted successfully " << endl;
                }
            }

            else
            {
                cout << "Account not found." << endl;
            }
        }

    }


    void saveAccountData()
    {
        Out.open(checkdatafile, ios::out);

        if (!Out)
        {

            return;
        }
        Customer* cust;
        CheckAccount* checkacc;
        for (int i = 0; i < size; i++)
        {
            cust = customer[i];
            checkacc = checkaccount[i];
            Out << left << setw(20) << checkacc->getName()
                << left << setw(20) << cust->getname()
                << left << setw(20) << cust->getpassword()
                << left << setw(20) << checkacc->getAccountNumber()
                << left << setw(20) << checkacc->getBalance()
                << left << setw(30) << checkacc->getphone()
                << endl;
        }

        Out.close();
    }
    void display() const
    {
        cout << "Account Type: Checking" << endl;
        cout << setw(50) << "=====================================CUSTOMER FOR CHECKING ACCOUNT LIST================================" << endl;
        cout << left << setw(15) << "A/C"
            << left << setw(18) << "Name"
            << left << setw(15) << "No. Account"
            << left << setw(20) << "Username"
            << left << setw(30) << "Phone No."
            << left << setw(5) << "Balance"
            << endl;

        for (int i = 0; i < size; i++)
        {
            cout << left << setw(15) << "Checking"
                << left << setw(18) << checkaccount[i]->getName() 
                << left << setw(15) << checkaccount[i]->getAccountNumber()
                << left << setw(20) << customer[i]->getname()
                << left << setw(30) << checkaccount[i]->getphone()
                << left << setw(5) << checkaccount[i]->getBalance()
                << endl;
        }
    }
    ~CheckAccount()
    {
        CheckAccount::saveAccountData();
        if (checkaccount != nullptr)
        {
            delete[] checkaccount;
        }
        if (customer != nullptr)
        {
            delete[] customer;
        }
    }
};

class AccountBuilder {
private:
    string name;
    int accountNumber;
    double balance;
    int phoneNumber;

public:
    AccountBuilder()
    {
        accountNumber = 0;
        name = "";
        balance = 0;
        phoneNumber = 0;
    }

    AccountBuilder& setName(const string& Name) {
        this->name = Name;
        return *this;
    }

    AccountBuilder& setAccountNumber(int accNumber) {
        this->accountNumber = accNumber;
        return *this;
    }

    AccountBuilder& setBalance(double bal) {
        this->balance = bal;
        return *this;
    }

    AccountBuilder& setPhoneNumber(int phoneNum) {
        this->phoneNumber = phoneNum;
        return *this;
    }

    SavingAccount* buildSavingAccount() {
        return new SavingAccount(name, accountNumber, balance, phoneNumber);
    }

    CheckAccount* buildCheckAccount() {
        return new CheckAccount(name, accountNumber, balance, phoneNumber);
    }
};


class Loan {
protected:
    int loanNumber;
    double loanAmount;
    double interestRate;
    string phonenumber;
    string name;

public:
    Loan(string name, int loanNumber, double loanAmount, double interestRate, string phonenumber)
    {
        this->name = name;
        this->loanNumber = loanNumber;
        this->loanAmount = loanAmount;
        this->interestRate = interestRate;
        this->phonenumber = phonenumber;
    }
    Loan() {
        loanNumber = 0;
        loanAmount = 0;
        interestRate = 0;
        phonenumber = "";
        name = "";
    }
    string getName()
    {
        return name;
    }

    void setName(string Name)
    {
        name = Name;
    }
    string getphone()
    {
        return phonenumber;
    }
    void setphone(string Num)
    {
        phonenumber = Num;
    }
    int getloannumber()
    {
        return loanNumber;
    }
    void setloannumber(int Number)
    {
        loanNumber = Number;
    }
    void setloanamount(double Amount)
    {
        loanAmount = Amount;
    }
    void setinterestrate(double rate)
    {
        interestRate = rate;
    }
    double getloanamount()
    {
        return loanAmount;
    }
    double getinterestrate()
    {
        return interestRate;
    }
    virtual void display() = 0;
    virtual void saveLoanData() = 0;
    virtual void calculateInterest() = 0;
    virtual void sreachaccount() = 0;

    virtual ~Loan() {}
};

class StudentFeeLoan : public Loan
{
private:
    string studentLoanDataFile;
    int size;

    Customer** customer;
    Admin admin;
    StudentFeeLoan** studentLoans;

public:
    StudentFeeLoan()
    {
        customer = nullptr;
        studentLoans = nullptr;
        size = 0;
    }
    StudentFeeLoan(string filename, int size)
    {
        this->studentLoanDataFile = filename;
        this->size = size;
        customer = new Customer * [size];
        studentLoans = new StudentFeeLoan * [size];

        for (int i = 0; i < size; i++)
        {
            studentLoans[i] = nullptr;
            customer[i] = nullptr;
        }

        Inp.open(filename);
        if (!Inp)
        {
            cout << "\t\t\tFailed to open Student Loan data file." << endl;
            return;
        }
        int index = 0;
        string line;
        while (getline(Inp, line))
        {
            stringstream ss(line);

            // Read data
            int loan_Number;
            double amountNO;
            double rate;
            string name;
            string enrollyear;
            string phone_numb;
            string user_name, Email;
            string pass_word;

            ss >> name >> user_name >> pass_word >> loan_Number >> amountNO >> rate >>  phone_numb;


            customer[index] = new Customer(user_name, pass_word);
            studentLoans[index] = new StudentFeeLoan(name, loan_Number, amountNO, rate, phone_numb);

            index++;
            if (index >= size)
            {

                break;
            }
        }

        Inp.close();
    }
    StudentFeeLoan(string name, int lnNumber, double amount, double rate, string phonenumber)
        : Loan(name, lnNumber, amount, rate, phonenumber)
    {
    }
   
    void display() override
    {
        std::cout << "Loan Type: Student Fee Loan" << std::endl;
        std::cout << "Loan Number: " << loanNumber << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Phone Number: " << phonenumber << std::endl;
        std::cout << "Loan Amount: $" << loanAmount << std::endl;
        std::cout << "Interest Rate: " << interestRate << "%" << std::endl;
    }

    void saveLoanData()
    {
        Out.open(studentLoanDataFile);

        if (Out.is_open())
        {

            for (int i = 0; i < size; i++)
            {
                Customer* cust = customer[i];
                StudentFeeLoan* loan = studentLoans[i];
                Out << left << setw(20) << loan->getName()
                    << left << setw(20) << cust->getname()
                    << left << setw(20) << cust->getpassword()
                    << left << setw(20) << loan->getloannumber()
                    << left << setw(20) << loan->getloanamount()
                    << left << setw(20) << loan->getinterestrate()
                    << left << setw(30) << loan->getphone()
                    << endl;
            }
        }
        else
        {
            return;
        }
        Out.close();
    }

    void calculateInterest()
    {
        int serach_account;
        cout << "Enter the Student Loan Number to serach: ";
        cin >> serach_account;
        bool found1 = false;

        int Index = -1;
        for (int i = 0; i < size; i++)
        {

            if (studentLoans[i]->getloannumber() == serach_account)
            {
                if (customer[i]->login())
                {
                    found1 = true;
                    Index = i;
                    break;
                }
            }
        }
        if (found1)
        {
            double interest = studentLoans[Index]->getloanamount() * studentLoans[Index]->getinterestrate() / 100;

            cout << "The total Interest on Student loan number " << studentLoans[Index]->getloannumber() << " is $ " << interest << endl;

        }
        else
        {
            cout << "\t\tStudent Loan Id not Found" << endl;

        }
    }

    void sreachaccount()
    {
        int serachaccount;
        cout << "\t\t\tEnter the Loan Number to serach: ";
        cin >> serachaccount;
        bool found1 = false;

        int Index = -1;
        for (int i = 0; i < size; i++)
        {
            if (studentLoans[i]->getloannumber() == serachaccount)
            {
                if (customer[i]->login())
                {
                    found1 = true;
                    Index = i;
                    break;
                }
            }
        }
        if (found1)
        {
            cout << left << setw(50) << "\t\tName: " << studentLoans[Index]->getName() << endl;
            cout << left << setw(50) << "\t\tUsername: " << customer[Index]->getname() << endl;
            cout << left << setw(50) << "\t\tLoan Number: " << studentLoans[Index]->getloannumber() << endl;
            cout << left << setw(50) << "\t\tLoan Amount: $" << studentLoans[Index]->getloanamount() << endl;
            cout << left << setw(50) << "\t\tInterest Rate: " << studentLoans[Index]->getinterestrate() << "%" << endl;
            cout << left << setw(50) << "\t\tPhone Number: " << studentLoans[Index]->getphone() << endl;
        }

        else
        {
            cout << "\t\tLoan number not found" << endl;
        }
    }
};

int CountLinesInFile(const string& filename)
{
    ifstream file(filename);
    int count = 0;
    string line;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            count++;
        }
    }
    file.close();
    return count;
}

class Menu {
public:
    void start() {
        system("cls");

        cout << "==================================================" << endl;
        cout << "   Welcome to the Bank Management System!         " << endl;
        cout << "==================================================" << endl;
        int savingSize = CountLinesInFile("save.txt");
        int checkSize = CountLinesInFile("check.txt");
        int studentSize = CountLinesInFile("student.txt");

        Account* SA = new SavingAccount("save.txt", savingSize);
        Account* checkAccount = new CheckAccount("check.txt", checkSize);
        Loan* studentLoan = new StudentFeeLoan("student.txt", studentSize);

        int choice;
        cout << "\t\tAccount Type \n";
        cout << endl;
        cout << "[1] . Administrator\n";
        cout << "[2] . User" << endl;
        cout << endl;
        cout << "Enter your Choice...!";
        cin >> choice;
        char c;
        switch (choice) {
        case(1):
            adminMenu(SA, savingSize);
            break;
        case(2):
            userMenu(SA, studentLoan);
            break;
        default:
            cout << "Invalid input" << endl;
            break;
        }
    }

private:
    void adminMenu(Account* SA, int savingSize) {
        Admin admin;
        int adminchoice;
        cout << "==================================================" << endl;
        cout << "\t\Welcom to Admin portal" << endl;
        cout << "==================================================" << endl;
        cout << "[1] View Customer Account" << endl;
        cout << "[2] Delete Customer Account" << endl;
        cout << "Select operation:";
        cin >> adminchoice;
        char x;
        switch (adminchoice) {
        case(1):
            system("cls");
            admin.login();
            SA -> display();
            cout << "\t\t\t\t\t\tEnter any Character to goto Main menu..... ";
            cin >> x;
            break;
        case (2):
            system("cls");
            SA -> deleteaccount("save.txt", savingSize);
            cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
            cin >> x;
            break;
        default:
            cout << "Invalid input" << endl;
            break;
        }
    }

    void userMenu(Account* SA, Loan* S_L) {
        int userchoice;
        cout << "==================================================" << endl;
        cout << "\t\t\Welcom to User Portal" << endl;
        cout << "==================================================" << endl;
        cout << "[1] . Cash Withdraw:\n";
        cout << "[2] . Cash Deposit:\n";
        cout << "[3] . Student Loan:\n";
        cout << "Select operation:";
        char x;
        cin >> userchoice;
        switch (userchoice)
        {
        case (1):
            system("cls");
            cout << "Enter the Ammount you want to withdraw ";
            double amount;
            cin >> amount;
            SA->withdraw(amount);
            cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
            cin >> x;
            break;
        case(2):
            system("cls");
            cout << "Enter the Ammount you want to Deposit ";
            double de_amount;
            cin >> de_amount;

            SA->deposit(de_amount);
            cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
            cin >> x;
            break;
        case (3):
            system("cls");
            cout << "\n\n";
            cout << "\t\t\t\t\t\t==================================================" << endl;
            cout << "\t\t\t\t\t\t\t\tStudent Loan Potral\n";
            cout << "\t\t\t\t\t\t==================================================" << endl;
            cout << "\n\t\t\t\t\t\t[1] . Loan Inquiry\n";
            cout << "\t\t\t\t\t\t[2] . Calculate Intrest:\n";

            int loanchoice;
            cout << "\n\t\t\t\t\t\t\tEnter your choice...";
            cin >> loanchoice;
            switch (loanchoice)
            {
            case 1:
                system("cls");
                S_L->sreachaccount();
                cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
                cin >> x;
                break;
            case 2:
                system("cls");
                S_L->calculateInterest();
                cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
                cin >> x;
                break;
            default:
                system("cls");
                cout << "\t\t\t\tInvalid Input" << endl;
                cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
                cin >> x;
                break;
            }
            break;

        default:
            cout << "Invalid input" << endl;
            break;
        }
    }
};


int main() {
    Menu menu;
    menu.start();
    return 0;
}


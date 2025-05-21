#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

class BankAccount
{
private:
    int AccountNum;
    double balance;
    string username;
    int password;

public:
    BankAccount(int a, string b, int c, double d)
    {
        AccountNum = a;
        username = b;
        password = c;
        balance = d;
    }

    void withdraw(double amount)
    {
        if (balance >= amount)
        {
            cout << "Withdrawing " << amount << "..." << endl;
            balance = balance - amount;
            updateBalance();
        }
        else
            cout << "No money";
    }

    void deposit(double amount)
    {
        cout << "Depositing " << amount << "..." << endl;
        balance = balance + amount;
        updateBalance();
    }

    void displayAccount()
    {
        cout << "\nAccount number: " << AccountNum << "\nAccount name: " << username << "\nBalance: " << balance << endl;
    }

    void displayBalance()
    {
        cout << "Current balance: " << balance << endl;
    }
    void updateBalance()
    {
        ifstream accountNums("AccountNums.txt");
        ifstream accountBalances("AccountBalance.txt");
        ofstream tempBalance("TempBalance.txt");

        if (!accountNums || !accountBalances || !tempBalance)
        {
            cout << "Error opening balance files!" << endl;
            return;
        }

        vector<int> accountNumbers;
        vector<double> balances;

        int num;
        double bal;
        while (accountNums >> num)
        {
            accountNumbers.push_back(num);
        }
        while (accountBalances >> bal)
        {
            balances.push_back(bal);
        }

        accountNums.close();
        accountBalances.close();

        if (accountNumbers.size() != balances.size())
        {
            cout << "Data inconsistency error!" << endl;
            return;
        }

        for (size_t i = 0; i < accountNumbers.size(); ++i)
        {
            if (accountNumbers[i] == AccountNum)
            {
                balances[i] = balance;
            }
            tempBalance << balances[i] << endl;
        }

        tempBalance.close();
        remove("AccountBalance.txt");
        rename("TempBalance.txt", "AccountBalance.txt");
    }

    void IRIS(int fromAcc)
    {
        // Display available accounts
        vector<int> accountNumbers;
        ifstream accountNums("AccountNums.txt");
        int accNum;
        cout << "Available accounts to transfer to:\n";
        while (accountNums >> accNum)
        {
            if (accNum != fromAcc)
            {
                accountNumbers.push_back(accNum);
                cout << " - " << accNum << endl;
            }
        }
        accountNums.close();

        if (accountNumbers.empty())
        {
            cout << "No other accounts available for transfer.\n";
            return;
        }

        // Ask for recipient account number
        int toAcc;
        cout << "Enter recipient account number: ";
        cin >> toAcc;
        while (cin.fail() || toAcc == fromAcc || std::find(accountNumbers.begin(), accountNumbers.end(), toAcc) == accountNumbers.end())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid account number. Please enter a valid recipient account number: ";
            cin >> toAcc;
        }

        // Ask for amount
        double amount;
        cout << "Enter amount to transfer: ";
        cin >> amount;
        while (cin.fail() || amount <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Please enter a positive number: ";
            cin >> amount;
        }

        // Now perform the transfer using the existing logic
        ifstream accountBalances("AccountBalance.txt");
        ofstream temporaryFile("TempBalance.txt");

        if (!accountBalances || !temporaryFile)
        {
            cout << "Error opening balance files!" << endl;
            return;
        }

        int lineNumber = 0;
        double balance;
        double fromBalance = -1, toBalance = -1;
        vector<double> balances;

        while (accountBalances >> balance)
        {
            balances.push_back(balance);
        }
        accountBalances.close();

        // Find indexes
        int fromIndex = -1, toIndex = -1;
        ifstream nums("AccountNums.txt");
        int num, idx = 0;
        while (nums >> num)
        {
            if (num == fromAcc)
                fromIndex = idx;
            if (num == toAcc)
                toIndex = idx;
            idx++;
        }
        nums.close();

        if (fromIndex == -1 || toIndex == -1)
        {
            cout << "Invalid source or destination account!" << endl;
            return;
        }

        fromBalance = balances[fromIndex];
        toBalance = balances[toIndex];

        if (fromBalance < amount)
        {
            cout << "Not enough balance to transfer!" << endl;
            return;
        }

        balances[fromIndex] -= amount;
        balances[toIndex] += amount;

        for (double b : balances)
        {
            temporaryFile << b << endl;
        }
        temporaryFile.close();

        remove("AccountBalance.txt");
        rename("TempBalance.txt", "AccountBalance.txt");

        cout << "Transferred $" << amount << " from account " << fromAcc
             << " to account " << toAcc << " successfully!" << endl;
    }

    void changePassword(int newPassword)
    {
        if (newPassword == password)
        {
            cout << "Cannot enter the same password." << endl;
            return;
        }

        password = newPassword;
        cout << "Password changed successfully!" << endl;

        ifstream accountPasswords("AccountPasswords.txt");
        ofstream temporaryFile("TempPasswords.txt");

        if (!accountPasswords || !temporaryFile)
        {
            cout << "Error opening files!" << endl;
            return;
        }

        int lineNumber = 0;
        int currentPass;
        while (accountPasswords >> currentPass)
        {
            if (lineNumber == AccountNum - 1)
            {
                temporaryFile << newPassword << endl;
            }
            else
            {
                temporaryFile << currentPass << endl;
            }
            lineNumber++;
        }

        accountPasswords.close();
        temporaryFile.close();

        remove("AccountPasswords.txt");
        rename("TempPasswords.txt", "AccountPasswords.txt");
    }
};

bool validation(int accountNum, int password, BankAccount &curAcc)
{
    ifstream accountNums("AccountNums.txt");
    ifstream accountNames("AccountNames.txt");
    ifstream accountPasswords("AccountPasswords.txt");
    ifstream accountBalances("AccountBalance.txt");
    if (!accountNums || !accountNames || !accountPasswords || !accountBalances)
    {
        cout << "Error opening files!" << endl;
        return 0;
    }
    int accNum, accPass;
    string accName;
    double accBalance;
    while (accountNums >> accNum && accountNames >> accName && accountPasswords >> accPass && accountBalances >> accBalance)
    {
        if (accountNum == accNum && password == accPass)
        {
            curAcc = BankAccount(accNum, accName, accPass, accBalance);
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    int accountNum, checkpassword, option, password, answer;
    double amount;
    BankAccount curAcc(0, "", 0, 0);
    cout << "Welcome to my banking project" << endl;
    while (true)
    {
        cout << "Do you have an account? (1 for Yes, 0 for No): ";
        cin >> answer;
        if (cin.fail() || (answer != 1 && answer != 0))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 for Yes or 0 for No." << endl;
        }
        else
        {
            break;
        }
    }

    if (answer == 1)
    {
        do
        {
            cout << "Please enter your account number: ";
            cin >> accountNum;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid account number." << endl;
                cin >> accountNum;
            }
            cout << "Please enter your account password: ";
            cin >> password;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid password." << endl;
                cin >> password;
            }
            if (validation(accountNum, password, curAcc) == false)
            {
                cout << "Invalid account number or password. Please try again.\n"
                     << endl;
            }
        } while (validation(accountNum, password, curAcc) == false);
        if (accountNum == 1 || accountNum == 2)
        {
            cout << ("\n");
            cout << "Login successful!" << endl;
            cout << "Welcome Admin";
            curAcc.displayAccount();
            cout << ("\n");
            cout << (" 1.Deposit\n 2.Withdraw\n 3.View Balance\n 4.IRIS\n 5.Change Password") << endl;
            cout << ("Give me your option:");
            cin >> option;
            while (cin.fail() || (option < 1 || option > 5))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << (" Invalid option: ") << endl;
                cout << (" 1.Deposit\n 2.Withdraw\n 3.View Balance\n 4.IRIS\n 5.Change Password") << endl;
                cout << (" Please give me your option again: ");
                cin >> option;
            }

            switch (option)
            {
            case 1:
            {
                cout << "Enter the amount: ";
                cin >> amount;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid amount." << endl;
                    cin >> amount;
                }
                curAcc.deposit(amount);
                curAcc.displayBalance();
                break;
            }
            case 2:
            {
                cout << "Enter the amount: ";
                cin >> amount;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid amount." << endl;
                    cin >> amount;
                }
                curAcc.withdraw(amount);
                curAcc.displayBalance();
                break;
            }
            case 3:
            {
                curAcc.displayBalance();
                break;
            }
            case 4:
            {
                int fromAccount;
                curAcc.IRIS(fromAccount);
                break;
            }

            case 5:
            {
                int newPassword;
                cout << ("Enter new password: ");
                cin >> newPassword;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid password." << endl;
                    cin >> newPassword;
                }
                curAcc.changePassword(newPassword);
                break;
            }
            }
        }
        else
        {
            cout << ("\n");
            cout << "Login successful!" << endl;
            cout << "Welcome User";
            curAcc.displayAccount();
            cout << ("\n");
            cout << (" 1.Deposit\n 2.Withdraw\n 3.View Balance\n 4.IRIS\n 5.Change Password") << endl;
            cout << ("Give me your option:");
            cin >> option;
            while (cin.fail() || (option < 1 || option > 5))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << (" Invalid option: ") << endl;
                cout << (" 1.Deposit\n 2.Withdraw\n 3.View Balance\n 4.IRIS\n 5.Change Password") << endl;
                cout << (" Please give me your option again: ");
                cin >> option;
            }

            switch (option)
            {
            case 1:
            {
                cout << "Enter the amount: ";
                cin >> amount;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid amount." << endl;
                    cin >> amount;
                }
                curAcc.deposit(amount);
                curAcc.displayBalance();
                break;
            }
            case 2:
            {
                cout << "Enter the amount: ";
                cin >> amount;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid amount." << endl;
                    cin >> amount;
                }
                curAcc.withdraw(amount);
                curAcc.displayBalance();
                break;
            }
            case 3:
            {
                curAcc.displayBalance();
                break;
            }
            case 4:
            {
                int fromAccount = accountNum;

                curAcc.IRIS(fromAccount);
                break;
            }

            case 5:
            {
                int newPassword;
                cout << ("Enter new password: ");
                cin >> newPassword;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid password." << endl;
                    cin >> newPassword;
                }
                curAcc.changePassword(newPassword);
                break;
            }
            }
        }
    }
    else
    {

        cout << "Please create an account" << endl;
        cout << "Enter your account number: ";
        cin >> accountNum;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid account number." << endl;
            cin >> accountNum;
        }
        bool hasLetter = false, valid = true;
        string name;
        cout << "Enter your name (letters and numbers allowed, but not only numbers): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);

        while (true)
        {

            valid = true;
            bool hasLetter = false;

            if (name.empty())
            {
                valid = false;
            }
            else
            {
                for (char ch : name)
                {
                    if (!isalnum(ch))
                    {
                        valid = false;
                        break;
                    }
                    if (isalpha(ch))
                    {
                        hasLetter = true;
                    }
                }
                if (!hasLetter)
                {
                    valid = false;
                }
            }

            if (valid)
                break;

            cout << "Invalid input. Please enter letters and numbers only, and at least one letter: ";
            getline(cin, name);
        }

        cout << "Enter your password: ";
        cin >> password;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid password." << endl;
            cin >> password;
        }
        cout << "Enter your balance: ";
        double balance;
        cin >> balance;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid balance." << endl;
            cin >> balance;
        }

        BankAccount newAccount(accountNum, name, password, balance);

        ofstream accountNums("AccountNums.txt", ios::app);
        ofstream accountNames("AccountNames.txt", ios::app);
        ofstream accountPasswords("AccountPasswords.txt", ios::app);
        ofstream accountBalances("AccountBalance.txt", ios::app);

        if (accountNums && accountNames && accountPasswords && accountBalances)
        {
            accountNums << accountNum << endl;
            accountNames << name << endl;
            accountPasswords << password << endl;
            accountBalances << balance << endl;

            cout << "Account created successfully!" << endl;
            newAccount.displayAccount();
        }
        else
        {
            cout << "Error creating account files!" << endl;
        }
    }
}

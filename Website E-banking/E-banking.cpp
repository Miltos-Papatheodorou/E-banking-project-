#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

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
        AccountNum=a;
        username=b;
        password=c;
        balance=d;
    }



void withdraw(double amount)
{
    if(balance>=amount)
    {
        cout<<"Withdrawing "<<amount<<"..."<<endl;
        balance=balance-amount;
    }
else
    cout<<"No money";
}



void deposit(double amount)
{
    cout<<"Depositing "<<amount<<"..."<<endl;
    balance=balance+amount;
}



void displayAccount()
{
    cout<<"\nAccount number: "<<AccountNum<<"\nAccount name: "<<username<<"\nBalance: "<<balance<<endl;
}



void displayBalance()
{
    cout<<"Current balance: "<<balance<<endl;
}

};

bool validation(int accountNum,int password,BankAccount &curAcc)
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
    while(accountNums >> accNum && accountNames >> accName &&accountPasswords >> accPass && accountBalances >> accBalance)
    {   
        if(accountNum== accNum && password == accPass)
        {
            curAcc=BankAccount(accNum, accName, accPass, accBalance);
            return true;
        }

    }
return false;
}

int main(int argc, char** argv)
{   
    int accountNum,password;
    BankAccount curAcc(0, "", 0, 0.0);
    cout<<"Welcome to my banking project"<<endl;
    do
        {
            cout << "Please enter your account number: ";
            cin >> accountNum;
            cout << "Please enter your account password: ";
            cin >> password;
            if (validation(accountNum,password,curAcc)==false)
            {   
                cout << "Invalid account number or password. Please try again.\n" << endl;
            }
        } while (validation(accountNum,password,curAcc)==false);
    cout << "Login successful!" << endl;
    curAcc.displayAccount();





}






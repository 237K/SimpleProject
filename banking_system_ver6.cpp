//
//  Banking_System_ver6.cpp
//  237Projects
//
//  Created by 형성모 on 29/12/2018.
//
//  Banking System 입니다.
//  ver2. 구조체를 클래스로 변경하고, 기존의 배열을 포인터 배열로 변경하였으며 동적할당으로 메모리사용을 최적화하였습니다.
//  ver3. ver2에서 발생한 segmentation fault를 해결하고, 복사생성자를 추가하였습니다.
//  ver4. 전역변수를 클래스 내부의 static 변수로 변경하여 혼동을 방지하였으며, const를 추가하여 안정성을 높였습니다.
//  ver5. 컨트롤 클래스를 만들어 전역변수와 전역함수를 모두 없앴습니다. 코드 가독성을 높였습니다.
//  ver6. 개설할 수 있는 계좌 종류를 추가하였습니다.(1. 보통예금계좌(고정저이율), 2. 신용계좌(신용도등급에 따라 높은 이자율을 지급하는 계좌))
//

#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

namespace MENU
{
    enum {OPEN = 1, DEPOSIT, WITHDRAW, VIEW, EXIT};
}

namespace CREDIT
{
    enum {LEVEL_A = 7, LEVEL_B = 4, LEVEL_C = 2};
}

//Account Class(Basic Class) 선언
class Account
{
private:
    int accountID;
    char *customer_name;
    int balance;
public:
    const static int NAME_LEN = 20;
public:
    Account(int account, char *name, int money);
    Account(Account & acc);
    int GetAccount() const;
    virtual void Deposit(int money);
    int Withdraw(int money);
    int GetBalance() const;
    void ShowAccountInfo() const;
    virtual void ShowAccount() const = 0;
    virtual ~Account();
};

//Account Class(Basic class) 정의
Account::Account(int account, char *name, int money) :accountID(account), balance(money)
{
    customer_name = new char[strlen(name)+1];
    strcpy(customer_name, name);
}
Account::Account(Account & acc) :accountID(acc.accountID), balance(acc.balance)
{
    customer_name = new char[strlen(acc.customer_name)+1];
    strcpy(customer_name, acc.customer_name);
}
int Account::GetAccount() const
{
    return accountID;
}
void Account::Deposit(int money)
{
    balance += money;
}
int Account::Withdraw(int money)
{
    if(balance < money) return 0;
    
    balance -= money;
    return money;
}
int Account::GetBalance() const
{
    return balance;
}
void Account::ShowAccountInfo() const
{
    cout<<"계좌번호 : "<<accountID<<endl;
    cout<<"이   름 : "<<customer_name<<endl;
    cout<<"잔   액 : "<<balance<<endl;
}
Account::~Account()
{
    delete []customer_name;
}

//고정금리 보통예금계좌 클래스(Derived class) 선언
class NormalAccount : public Account
{
private:
    int interest_ratio;
public:
    NormalAccount(int account, char *name, int money, int interest);
    int GetBasicInterest(int money);
    virtual void Deposit(int money);
    int GetBasicInterestRatio() const;
    void ShowAccount() const;
    ~NormalAccount() {}
};

//고정금리 보통예금계좌 클래스(Derived class) 정의
NormalAccount::NormalAccount(int account, char *name, int money, int interest) :Account(account, name, money), interest_ratio(interest) {}
int NormalAccount::GetBasicInterest(int money)
{
    return (Account::GetBalance() + money) * (interest_ratio/100.0);
}
void NormalAccount::Deposit(int money)
{
    Account::Deposit(money);
    Account::Deposit(GetBasicInterest(money));
}
int NormalAccount::GetBasicInterestRatio() const
{
    return interest_ratio;
}
void NormalAccount::ShowAccount() const
{
    cout<<"[보통예금계좌]"<<endl;
    ShowAccountInfo();
    cout<<"기본이자율 : "<<interest_ratio<<endl<<endl;
}

//우대금리 신용계좌 클래스(Derived class) 선언
class HighCreditAccount : public NormalAccount
{
private:
    int credit_level;
public:
    HighCreditAccount(int account, char *name, int money, int interest, int credit);
    int GetAdditionalInterest(int money);
    virtual void Deposit(int money);
    int GetCreditLevel() const;
    void ShowAccount() const;
    ~HighCreditAccount() {}
};

//우대금리 신용계좌 클래스(Derived class) 정의
HighCreditAccount::HighCreditAccount(int account, char *name, int money, int interest, int credit) :NormalAccount(account, name, money, interest), credit_level(credit) {}
    
int HighCreditAccount::GetAdditionalInterest(int money)
{
    return (Account::GetBalance() + money) * (credit_level/100.0);
}
void HighCreditAccount::Deposit(int money)
{
    NormalAccount::Deposit(money);
    Account::Deposit(GetAdditionalInterest(money));
}
int HighCreditAccount::GetCreditLevel() const
{
    return credit_level;
}
void HighCreditAccount::ShowAccount() const
{
    cout<<"[고이율신용계좌]"<<endl;
    ShowAccountInfo();
    cout<<"기본이자율 : "<<NormalAccount::GetBasicInterestRatio()<<endl;
    cout<<"신용등급에 따른 우대이자율 : "<<GetCreditLevel()<<endl<<endl;
}

//컨트롤 클래스 입니다.
class AccountHandler
{
private:
    Account *customer[100];
public:
    static int AccountNumberCount;
public:
    AccountHandler() {}
    ~AccountHandler();
    void ShowMenu() const;
    void OpenAccount();
    void OpenNormalAccount();
    void OpenHighCreditAccount();
    void DepositMoney();
    void WithdrawMoney();
    void ShowAllAccount() const;
};
int AccountHandler::AccountNumberCount = 0;        //static 변수 초기화

void AccountHandler::ShowMenu() const
{
    cout<<"---------Menu---------"<<endl;
    cout<<"1. 계좌개설"<<endl;
    cout<<"2. 입   금"<<endl;
    cout<<"3. 출   금"<<endl;
    cout<<"4. 계좌정보 전체 출력"<<endl;
    cout<<"5. 프로그램 종료"<<endl;
}
void AccountHandler::OpenAccount()
{
    int select = 0;
    cout<<"1. 보통예금계좌, 2. 신용계좌"<<endl;
    cout<<"*어떤 종류의 계좌를 개설하시겠습니까? "; cin>>select;
    cout<<endl;
    if(select == 1) OpenNormalAccount();
    if(select == 2) OpenHighCreditAccount();
    if(select != 1 && select != 2)
    {
        cout<<"다시 선택해주십시오."<<endl;
        return;
    }
}
void AccountHandler::OpenNormalAccount()
{
    int myaccount = 0, mymoney = 0, interest = 0;
    char myname[Account::NAME_LEN];
    cout<<"[계좌개설]"<<endl;
    cout<<"계좌번호 : ";
    cin>>myaccount;
    cout<<"이   름 : ";
    cin>>myname;
    cout<<"입금금액 : ";
    cin>>mymoney;
    cout<<"이자율 : ";
    cin>>interest;
    cout<<endl;
    
    customer[AccountNumberCount++] = new NormalAccount(myaccount, myname, mymoney, interest);
}
void AccountHandler::OpenHighCreditAccount()
{
    int myaccount = 0, mymoney = 0, interest = 0, credit;
    char myname[Account::NAME_LEN];
    cout<<"[계좌개설]"<<endl;
    cout<<"계좌번호 : ";
    cin>>myaccount;
    cout<<"이   름 : ";
    cin>>myname;
    cout<<"입금금액 : ";
    cin>>mymoney;
    cout<<"이자율 : ";
    cin>>interest;
    cout<<"신용등급(1. LEVEL A (7%), 2. LEVEL B (4%), 3. LEVEL C (2%) : ";
    cin>>credit;
    cout<<endl;
    
    switch(credit)
    {
    case 1:
        customer[AccountNumberCount++] = new HighCreditAccount(myaccount, myname, mymoney, interest, CREDIT::LEVEL_A);
        cout<<"성공적으로 계좌를 개설했습니다."<<endl;
        break;
    case 2:
        customer[AccountNumberCount++] = new HighCreditAccount(myaccount, myname, mymoney, interest, CREDIT::LEVEL_B);
        cout<<"성공적으로 계좌를 개설했습니다."<<endl;
        break;
    case 3:
        customer[AccountNumberCount++] = new HighCreditAccount(myaccount, myname, mymoney, interest, CREDIT::LEVEL_C);
        cout<<"성공적으로 계좌를 개설했습니다."<<endl;
        break;
    }
}
void AccountHandler::DepositMoney()
{
    int myaccount = 0, money = 0;
    cout<<"[입   금]"<<endl;
    cout<<"계좌번호 : ";
    cin>>myaccount;
    cout<<"입금금액 : ";
    cin>>money;
    cout<<endl;
    
    if(money <= 0) cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
    for(int i = 0; i < AccountNumberCount; i++)
    {
        if(customer[i]->GetAccount() == myaccount)
        {
            customer[i]->Deposit(money);
            cout<<"*입금완료"<<endl;
            return;
        }
    }
    cout<<"일치하는 계좌정보가 없습니다."<<endl;
}
void AccountHandler::WithdrawMoney()
{
    int myaccount = 0, money = 0;
    cout<<"[출   금]"<<endl;
    cout<<"계좌번호 : ";
    cin>>myaccount;
    cout<<"출금금액 : ";
    cin>>money;
    cout<<endl;
    
    if(money <= 0) cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
        
    for(int i = 0; i < AccountNumberCount; i++)
    {
        if(customer[i]->GetAccount() == myaccount)
        {
            if(customer[i]->Withdraw(money) == 0)
            {
                cout<<"잔액이 부족합니다."<<endl;
                return;
            }
            cout<<"*출금완료"<<endl<<endl;
            return;
        }
    }
    cout<<"일치하는 계좌정보가 없습니다."<<endl;
}
void AccountHandler::ShowAllAccount() const
{
    for(int i = 0; i < AccountNumberCount; i++)
    {
        customer[i]->ShowAccount();
        cout<<endl;
    }
}
AccountHandler::~AccountHandler()
{
    for(int i = 0; i < AccountNumberCount; i++)
    {
        delete customer[i];
    }
}

//main
int main(void)
{
    int Select = 0;
    AccountHandler manager;
    
    while(1)
    {
        manager.ShowMenu();
        cout<<endl<<"메뉴를 선택해주세요.";
        cin>>Select;
        cout<<endl;
        
        switch(Select)
        {
            case MENU::OPEN:
                manager.OpenAccount();
                break;
            case MENU::DEPOSIT:
                manager.DepositMoney();
                break;
            case MENU::WITHDRAW:
                manager.WithdrawMoney();
                break;
            case MENU::VIEW:
                manager.ShowAllAccount();
                break;
            case MENU::EXIT:
                return 0;
            default:
                cout<<"다시 번호를 입력해주세요."<<endl;
        }
    }
    return 0;
}

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

class Account
{
private:
    int accountID;
    char *customer_name;
    int balance;
public:
    const static int NAME_LEN = 20;
public:
    Account(int account, char *name, int money) :accountID(account), balance(money)
    {
        customer_name = new char[strlen(name)+1];
        strcpy(customer_name, name);
    }
    Account(Account & acc) :accountID(acc.accountID), balance(acc.balance)
    {
        customer_name = new char[strlen(acc.customer_name)+1];
        strcpy(customer_name, acc.customer_name);
    }
    
    int GetAccount() const
    {
        return accountID;
    }
    
    int GetBalance() const
    {
        return balance;
    }
    
    void SetBalance(int money)
    {
        balance += money;
    }

    void ShowAccountInfo() const
    {
        cout<<"계좌번호 : "<<accountID<<endl;
        cout<<"이   름 : "<<customer_name<<endl;
        cout<<"잔   액 : "<<balance<<endl;
    }
    
    virtual void Deposit(int money) = 0;
    virtual int Withdraw(int money) = 0;
    virtual void ShowAccount() const = 0;
    virtual ~Account()
    {
        delete []customer_name;
    }
};

class NormalAccount : public Account
{
private:
    int interest_ratio;
public:
    NormalAccount(int account, char *name, int money, int interest) :Account(account, name, money), interest_ratio(interest) {}
    ~NormalAccount() {}

    int GetInterestRatio() const
    {
        return interest_ratio;
    }
    
    int GetBasicInterest(int money)
    {
        return (Account::GetBalance() + money) * (int)(interest_ratio/100);
    }
    
    void Deposit(int money)
    {
        Account::SetBalance(GetBasicInterest(money));
    }
    
    int Withdraw(int money)
    {
        if(Account::GetBalance() < money) return 0;
        
        Account::SetBalance(-money);
        return money;
    }
    
    void ShowAccount() const
    {
        cout<<"[보통예금계좌]"<<endl;
        ShowAccountInfo();
        cout<<"기본이자율 : "<<interest_ratio<<endl<<endl;
    }
};

class HighCreditAccount : public NormalAccount
{
private:
    int credit_level;
public:
    HighCreditAccount(int account, char *name, int money, int interest, int credit) :NormalAccount(account, name, money, interest), credit_level(credit) {}
    ~HighCreditAccount() {}
    
    int GetAdditionalInterest(int money)
    {
        return (Account::GetBalance() + money) * (int)(credit_level/100);
    }
    
    void Deposit(int money)
    {
        Account::SetBalance(NormalAccount::GetBasicInterest(money) + GetAdditionalInterest(money));
    }
    
    int Withdraw(int money)
    {
        if(Account::GetBalance() < money) return 0;
        
        Account::SetBalance(-money);
        return money;
    }
    
    void ShowAccount() const
    {
        cout<<"[고이율신용계좌]"<<endl;
        ShowAccountInfo();
        cout<<"기본이자율 : "<<GetInterestRatio()<<endl;
        cout<<"신용등급에 따른 우대이자율 : "<<credit_level<<endl<<endl;
    }
};

//Control Class
class AccountHandler
{
private:
    Account *customer[100];
public:
    static int AccountNumberCount;
public:
    AccountHandler() {}
    ~AccountHandler()
    {
        for(int i = 0; i < AccountNumberCount; i++)
        {
            delete customer[i];
        }
    }
    
    void AddAccount(Account *acc)
    {
        customer[AccountNumberCount++] = acc;
        cout<<"계좌가 성공적으로 개설되었습니다."<<endl<<endl;
    }
    
    void ShowMenu() const
    {
        cout<<"---------Menu---------"<<endl;
        cout<<"1. 계좌개설"<<endl;
        cout<<"2. 입   금"<<endl;
        cout<<"3. 출   금"<<endl;
        cout<<"4. 계좌정보 전체 출력"<<endl;
        cout<<"5. 프로그램 종료"<<endl;
    }
    
    void OpenAccount()
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
    
    void OpenNormalAccount()
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
        
        NormalAccount *normal_acc = new NormalAccount(myaccount, myname, mymoney, interest);
        AddAccount(normal_acc);
    }
    
    void OpenHighCreditAccount()
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
        cout<<"신용등급(1 = LEVEL A (7%), 2 = LEVEL B (4%), 3 = LEVEL C (2%) : ";
        cin>>credit;
        cout<<endl;
        
        HighCreditAccount *credit_acc = new HighCreditAccount(myaccount, myname, mymoney, interest, credit);
        AddAccount(credit_acc);
    }
    
    void DepositMoney()
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
    
    void WithdrawMoney()
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
    
    void ShowAllAccount() const
    {
        for(int i = 0; i < AccountNumberCount; i++)
        {
            customer[i]->ShowAccount();
            cout<<endl;
        }
    }
};
int AccountHandler::AccountNumberCount = 0;        //static 변수 초기화

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

//
//  BangkingSystem_ver7.cpp
//  237Projects
//
//  Created by 형성모 on 04/01/2019.
//
//  Banking System 입니다.
//  ver2. 구조체를 클래스로 변경하고, 기존의 배열을 포인터 배열로 변경하였으며 동적할당으로 메모리사용을 최적화하였습니다.
//  ver3. ver2에서 발생한 segmentation fault를 해결하고, 복사생성자를 추가하였습니다.
//  ver4. 전역변수를 클래스 내부의 static 변수로 변경하여 혼동을 방지하였으며, const를 추가하여 안정성을 높였습니다.
//  ver5. 컨트롤 클래스를 만들어 전역변수와 전역함수를 모두 없앴습니다. 코드 가독성을 높였습니다.
//  ver6. 개설할 수 있는 계좌 종류를 추가하였습니다.(1. 보통예금계좌(고정저이율), 2. 신용계좌(신용도등급에 따라 높은 이자율을 지급하는 계좌))
//  ver7. String class를 추가하였습니다.
//  ver8. 배열의 인덱스가 범위를 초과하는 경우를 방지하기 위한 BoundCheckArray 클래스를 구현하여 customer배열에 적용하였습니다.
//  ver9. BoundCheckArray클래스를 템플릿화함으로써 다른 클래스들에서도 사용할 수 있도록 일반화하였습니다.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;

namespace MENU
{
    enum {OPEN = 1, DEPOSIT, WITHDRAW, VIEW, EXIT};
}

namespace CREDIT
{
    enum {LEVEL_A = 7, LEVEL_B = 4, LEVEL_C = 2};
}

//BoundCheckArray Class 선언
//class Account;                                  //typedef를 위해 나중에 Account 클래스를 정의한다는 것을 컴파일러에게 알림
//typedef Account* ACCOUNT_PTR;                   //Account를 상속하는 클래스의 객체들도 저장해야하므로 Account객체의 주소를 저장하기 위함
template <class T>
class BoundCheckArray
{
private:
    T *array;
    int index;
    int array_len;
public:
    BoundCheckArray(int len=100);
    ~BoundCheckArray();
    T& operator[](int index);
    T operator[](int index) const;
};

//BoundCheckArray Class 정의
template <class T>
BoundCheckArray<T>::BoundCheckArray(int len) : index(0), array_len(len)
{
    array = new T[len];
}
template <class T>
BoundCheckArray<T>::~BoundCheckArray() { delete []array; }
template <class T>
T& BoundCheckArray<T>::operator[](int index)
{
    if(index < 0 || index >= array_len)
    {
        cout<<"Array index out of bound exception"<<endl;
        exit(1);
    }
    return array[index];
}
template <class T>
T BoundCheckArray<T>::operator[](int index) const
{
    if(index < 0 || index >= array_len)
    {
        cout<<"Array index out of bound exception"<<endl;
        exit(1);
    }
    return array[index];
}

//String Class 선언
class String
{
private:
    char *str;
    int len;
public:
    String();
    String(const char *str);
    String(const String& cpy);
    String& operator=(const String &assign);
    String operator+(const String &str1);
    String& operator+=(const String &str1);
    bool operator==(const String &str1);
    ~String();
    friend ostream& operator<<(ostream& os, const String &str1);
    friend istream& operator>>(istream& is, String &str1);
};

//String Class 정의
String::String() : len(0)
{
    str = NULL;
}
String::String(const char *str)
{
    len = (int)(strlen(str)+1);
    this->str = new char[len];
    strcpy(this->str, str);
}
String::String(const String& cpy)
{
    this->len = cpy.len;
    this->str = new char[len];
    strcpy(this->str, cpy.str);
}
String& String::operator=(const String &assign)
{
    if(str != NULL) delete []str;
    len = assign.len;
    this->str = new char[len];
    strcpy(this->str, assign.str);
    return *this;
}
String String::operator+(const String &str1)
{
    char *temp = new char[len + str1.len - 1];
    strcpy(temp, str);
    strcat(temp, str1.str);
    String Temp(temp);
    delete []temp;
    return Temp;
}
String& String::operator+=(const String &str1)
{
    this->len += str1.len;
    char *temp = new char[len];
    strcpy(temp, str);
    strcat(temp, str1.str);
    
    if(str != NULL) delete []str;
    str = temp;
    return *this;
}
bool String::operator==(const String &str1)
{
    return strcmp(str, str1.str) ? false : true;
}
String::~String()
{
    if(str != NULL) delete []str;
}

ostream& operator<<(ostream& os, const String &str1)
{
    os<<str1.str;
    return os;
}

istream& operator>>(istream& is, String &str1)
{
    char *temp = new char[100];
    is>>temp;
    String Temp(temp);
    str1 = Temp;
    delete []temp;
    return is;
}

//Account Class(Basic Class) 선언
class Account
{
private:
    int accountID;
    String customer_name;
    int balance;
public:
    const static int NAME_LEN = 20;
public:
    Account(int account, String name, int money);
    Account(Account & acc);
    Account& operator=(const Account &assign);
    int GetAccount() const;
    virtual void Deposit(int money);
    int Withdraw(int money);
    int GetBalance() const;
    void ShowAccountInfo() const;
    virtual void ShowAccount() const = 0;
    virtual ~Account();
};

//Account Class(Basic class) 정의
Account::Account(int account, String name, int money) :accountID(account), balance(money)
{
    customer_name = name;
}
Account::Account(Account & acc) :accountID(acc.accountID), balance(acc.balance)
{
    customer_name = acc.customer_name;
}
Account& Account::operator=(const Account &assign)
{
    accountID = assign.accountID;
    customer_name = assign.customer_name;
    balance = assign.balance;
    return *this;
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
Account::~Account() {}

//고정금리 보통예금계좌 클래스(Derived class) 선언
class NormalAccount : public Account
{
private:
    int interest_ratio;
public:
    NormalAccount(int account, String name, int money, int interest);
    NormalAccount& operator=(NormalAccount& assign);
    int GetBasicInterest(int money);
    virtual void Deposit(int money);
    int GetBasicInterestRatio() const;
    void ShowAccount() const;
    ~NormalAccount() {}
};

//고정금리 보통예금계좌 클래스(Derived class) 정의
NormalAccount::NormalAccount(int account, String name, int money, int interest) :Account(account, name, money), interest_ratio(interest) {}
NormalAccount& NormalAccount::operator=(NormalAccount &assign)
{
    Account::operator=(assign);
    interest_ratio = assign.interest_ratio;
    return *this;
}
int NormalAccount::GetBasicInterest(int money)
{
    return money * (interest_ratio/100.0);
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
    HighCreditAccount(int account, String name, int money, int interest, int credit);
    HighCreditAccount& operator=(HighCreditAccount& assign);
    int GetAdditionalInterest(int money);
    virtual void Deposit(int money);
    int GetCreditLevel() const;
    void ShowAccount() const;
    ~HighCreditAccount() {}
};

//우대금리 신용계좌 클래스(Derived class) 정의
HighCreditAccount::HighCreditAccount(int account, String name, int money, int interest, int credit) :NormalAccount(account, name, money, interest), credit_level(credit) {}
HighCreditAccount& HighCreditAccount::operator=(HighCreditAccount &assign)
{
    Account::operator=(assign);
    credit_level = assign.credit_level;
    return *this;
}
int HighCreditAccount::GetAdditionalInterest(int money)
{
    return money * (credit_level/100.0);
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
    BoundCheckArray<Account*> customer;
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
    String myname;
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
    String myname;
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
    
    if(money <= 0)
    {
        cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
        return;
    }
    
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

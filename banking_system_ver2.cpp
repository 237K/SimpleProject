//
//  main.cpp
//  237Projects
//
//  Created by 형성모 on 2018. 12. 26..
//
//  Banking System 입니다.
//  ver2. 구조체를 클래스로 변경하고, 기존의 배열을 포인터 배열로 변경하였으며 동적할당으로 메모리사용을 최적화하였습니다.

#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

//Fuction Protocol
void open_account(void);
void DepositM(void);
void WithdrawM(void);
void view_balance(void);
void print_menu(void);

namespace MENU_SELECT
{
    enum {OPEN = 1, DEPOSIT, WITHDRAW, VIEW, EXIT};
}

//Class 선언
class Customers
{
private:
    int account;
    char *name;
    int balance;
public:
    Customers(){}
    Customers(const Customers &cus);
    Customers(int account, char *name, int balance);
    ~Customers();
    int GetAccount();
    void deposit(int balance);
    int ShowBalance() const;
    int withdraw(int balance);
    char* ShowName() const;
};

//Class 함수 정의
Customers::Customers(const Customers &cus)
{
    account = cus.account;
    name = new char[strlen(cus.name)+1];
    strcpy(name, cus.name);
    balance = cus.balance;
}

Customers::Customers(int account, char *name, int balance)
{
    this->account = account;
    this->name = new char[strlen(name)+1];
    strcpy(this->name, name);
    this->balance = balance;
    cout<<"새로운 고객의 계좌 생성을 완료하였습니다!"<<endl<<endl;
}

Customers::~Customers()
{
    delete []name;
    cout<<"고객 정보에 대한 동적할당을 해제하였습니다."<<endl<<endl;
}

int Customers::GetAccount()
{
    return this->account;
}

void Customers::deposit(int balance)
{
    this->balance += balance;
}

int Customers::ShowBalance() const
{
    return this->balance;
}

int Customers::withdraw(int balance)
{
    if(this->balance < balance) return 0;
    
    this->balance -= balance;
    return this->balance;
}

char* Customers::ShowName() const
{
    return this->name;
}

    
//전역변수 선언
const int NAME_LEN = 20;
int count = 0;
Customers *customer[100];

//main
int main(void)
{
    
    int select_num = 0;
    
    while(1)
    {
        print_menu();
        cout<<endl<<"메뉴를 선택해주세요.";
        cin>>select_num;
        cout<<endl;

        switch (select_num)
        {
            case MENU_SELECT::OPEN : open_account();
                break;
            case MENU_SELECT::DEPOSIT : DepositM();
                break;
            case MENU_SELECT::WITHDRAW : WithdrawM();
                break;
            case MENU_SELECT::VIEW : view_balance();
                break;
            case MENU_SELECT::EXIT :
                for(int i = 0; i < count; i++)
                {
                    delete customer[i];
                }
                return 0;
            default :
                cout<<"다시 번호를 입력해주세요."<<endl;
        }
    }
    return 0;
}

void print_menu(void)
{
    cout<<"-----Menu-----"<<endl;
    cout<<"1. 계좌개설"<<endl;
    cout<<"2. 입   금"<<endl;
    cout<<"3. 출   금"<<endl;
    cout<<"4. 계좌정보 전체 출력"<<endl;
    cout<<"5. 프로그램 종료"<<endl;
}

void open_account(void)
{
    int account_num, open_balance;
    char customer_name[NAME_LEN];
    
    cout<<"[개좌개설]"<<endl;
    cout<<"계좌번호 : ";
    cin>>account_num;
    cout<<"이   름 : ";
    cin>>customer_name;
    cout<<"입금금액 : ";
    cin>>open_balance;
    cout<<endl;
    
    customer[count+1] = new Customers(account_num, customer_name, open_balance);
    
    count++;
}

void DepositM(void)
{
    int temp_acc = 0, temp_bal = 0;
    
    cout<<endl<<"[입   금]"<<endl;
    cout<<"계좌번호 : ";
    cin>>temp_acc;
    cout<<"입금금액 : ";
    cin>>temp_bal;
    
    if(temp_bal < 0) cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
    
    for(int i = 0; i < count; i++)
    {
        if(customer[i]->GetAccount() == temp_acc)
        {
            customer[i]->deposit(temp_bal);
            cout<<endl<<"*입금 완료"<<endl;
            return;
        }
    }
    cout<<"일치하는 계좌가 없습니다."<<endl;
}

void WithdrawM(void)
{
    int temp_acc = 0, temp_bal = 0;
    
    cout<<endl<<"[출   금]"<<endl;
    cout<<"계좌번호 : ";
    cin>>temp_acc;
    cout<<"출금금액 : ";
    cin>>temp_bal;
    
    if(temp_bal < 0) cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;

    for(int i = 0; i < count; i++)
    {
        if(customer[i]->GetAccount() == temp_acc)
        {
            if(customer[i]->withdraw(temp_bal) == 0)
            {
                cout<<"잔액이 충분하지 않습니다."<<endl;
                return;
            }
            cout<<"*출금 완료"<<endl;
            return;
        }
    }
    cout<<"일치하는 계좌가 없습니다."<<endl;
}
          

void view_balance(void)
{
    cout<<"전체 계좌 수 : "<<count<<endl;
    
    for(int i = 0; i < count; i++)
    {
        cout<<"--------------------"<<endl;
        cout<<"계좌번호 : "<<customer[i]->GetAccount()<<endl;
        cout<<"이   름 : "<<customer[i]->ShowName()<<endl;
        cout<<"잔   액 : "<<customer[i]->ShowBalance()<<endl;
        cout<<"--------------------"<<endl;
    }
}

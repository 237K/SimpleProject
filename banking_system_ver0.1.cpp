//
//  main.cpp
//  237Projects
//
//  Created by 형성모 on 2018. 12. 26..
//
//  Banking System 입니다.

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

//fuction protocol
void open_account(void);
void deposit(void);
void withdraw(void);
void view_balance(void);
void print_menu(void);

//global variable
struct customers
{
    int account;
    int balance;
    char name[10];
} customer[100];

//main

int main(void)
{
    int select_num = 0;
    
    while(1)
    {
        print_menu();
        cin>>select_num;
        if(select_num > 0 && select_num < 6)
        {
            switch (select_num)
            {
                case 1 : open_account();
                    break;
                case 2 : deposit();
                    break;
                case 3 : withdraw();
                    break;
                case 4 : view_balance();
                    break;
                case 5 : return 0;
                default :
                    cout<<"Illegal selection.."<<endl;
            }
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
    int i = 0;
    while (i < 100)
    {
        if (customer[i].account != 0) i++;
        
        else if(customer[i].account == 0)
        {
            cout<<"[개좌개설]"<<endl;
            cout<<"계좌번호 : ";
            cin>>customer[i].account;
            cout<<"이   름 : ";
            cin>>customer[i].name;
            cout<<"입금금액 : ";
            cin>>customer[i].balance;
            return;
        }
    }
}

void deposit(void)
{
    int temp = 0, temp_bal = 0;
    
    cout<<"계좌번호 : ";
    cin>>temp;
    cout<<"입금금액 : ";
    cin>>temp_bal;
    
    if(temp_bal > 0)
    {
        for(int i = 0; i < 100; i++)
        {
            if(customer[i].account == temp)
            {
                customer[i].balance += temp_bal;
                cout<<"입금완료"<<endl;
                return;
            }
        }
        cout<<"일치하는 계좌정보가 없습니다."<<endl;
    }
    else cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
}

void withdraw(void)
{
    int temp = 0, temp_bal = 0;
    
    cout<<"계좌번호 : ";
    cin>>temp;
    cout<<"출금금액 : ";
    cin>>temp_bal;
    
    if(temp_bal > 0)
    {
        for(int i = 0; i < 100; i++)
        {
            if(customer[i].account == temp)
            {
                if(customer[i].balance > temp_bal)
                {
                    customer[i].balance -= temp_bal;
                    cout<<"출금 완료"<<endl;
                    return;
                }
                else
                {
                    cout<<"잔액이 충분하지 않습니다."<<endl;
                    return;
                }
            }
        }
        cout<<"일치하는 계좌정보가 없습니다."<<endl;
    }
    else cout<<"0원 이상의 금액을 입력하여 주십시오."<<endl;
    
}

void view_balance(void)
{
    for(int i = 0; i < 100; i++)
    {
        if(customer[i].account != 0)
        {
            cout<<"계좌번호 : "<<customer[i].account<<endl;
            cout<<"이   름 : "<<customer[i].name<<endl;
            cout<<"잔   액 : "<<customer[i].balance<<endl;
        }
    }
}

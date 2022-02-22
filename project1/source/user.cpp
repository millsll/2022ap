#include"../includes/user.h"
#include<iostream>
using namespace std;
void User::user_log_in(){
    string in_account,in_password;
    bool flag=false;
    do{
        cout<<"输入用户账号：";
        cin>>in_account;
        cout<<"输入用户密码：";
        cin>>in_password;

        //查询用户账号密码
        flag=match_account(in_account,in_password);

        //匹配错误
        if(!flag){
            string option;
            cout<<"账号或密码错误，要重试吗？(y/n)";
            do{
                cin>>option;
                if(option!="y"&&option!="n"&&option!="Y"&&option!="N")
                    cout<<"请重新输入(y/n)";
            }while(option!="y"&&option!="n"&&option!="Y"&&option!="N");
            if(option=="N"||option=="n"){
                cout<<"登录失败，返回初始界面。"<<endl;
                break;//跳出输入循环，离开user login界面
            }
        }
    }while(!flag);

    if(flag){
        cout<<"登录成功！"<<endl;
        //TODO: laod uer info
        user_id=in_account;
        //进入主界面
        user_interface();
    }
    //
}
void User::user_log_out(){
    //unload user data
    unload_data();
    user_id="";
}
void User::user_info_function(){
    while(1){
        cout<<"个人信息管理"<<endl;
        cout<<"1.查看信息 2.修改信息 3.充值 4.返回用户界面"<<endl;
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4");

        if(option=="1"){
            //seach user_info
        }
        else if(option=="2"){
            //update user_info
        }
        else if(option=="3"){
            //charge
        }
        else if(option=="4"){
            //跳出个人信息界面，返回用户界面
            break;
        }
    }
}
void User::user_interface(){
    while(1){
        cout<<"请选择功能"<<endl;
        cout<<"1、我是买家 2、我是卖家 3、个人信息 4、注销登录"<<endl;
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4");

        
        if(option=="1"){
            //load buyer data;
            init_buyer(user_id);
            //show buyer funtions
        }
        else if(option=="2"){
            //load seller data
            init_seller(user_id);
            //show seller functions
        }
        else if (option=="3"){
            //show info function
            user_info_function();
        }
        else if(option=="4"){
            //清除数据
            user_log_out();
            //跳出用户界面循环，返回主界面；
            break;
        }
    }
}
#include"../includes/deal_sys.h"
#include<iostream>
using namespace std;
void Deal_sys::sys_log_in(){
    cout<<"**************************欢迎使用冬奥纪念品交易系统*******************"<<endl;
    cout<<"请选择功能："<<endl;
    cout<<"1、用户登录 2、管理员登录 3、用户注册 4、退出系统"<<endl;
    string option;
    do{
        cin>>option;
        if(option!="1"&&option!="2"&&option!="3"&&option!="4")
            cout<<"选项错误，请重新选择：";
    }while(option!="1"&&option!="2"&&option!="3"&&option!="4");
    if(option=="1"){
        user.user_log_in();
    }
    else if(option=="2"){
        admin.admin_log_in();
    }
    else if(option=="3"){
        user.user_register();
    }
    else if(option=="4"){
        exit(0);
    }

    
}
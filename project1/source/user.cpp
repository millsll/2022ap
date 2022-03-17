#include"../includes/user.h"
#include<iostream>
using namespace std;
void User::user_log_in(){
    string in_account,in_password;
    int flag=0;
    do{
        cout<<"输入用户账号：";
        cin>>in_account;
        cout<<"输入用户密码：";
        cin>>in_password;

        //查询用户账号密码,并加载用户信息
        flag=match_account(in_account,in_password);

        //匹配错误 flag=0;
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
        else if(flag==1){
            //账号已封禁
            cout<<"该账号已被封禁，不能登录"<<endl;
        }
    }while(!flag);

    if(flag==2){
        cout<<"登录成功！"<<endl;
        //TODO: laod uer info
        user_id=in_account;
        load_user_data(user_id);
        //进入主界面
        //cout<<"加载成功"<<endl;
        user_interface();
    }
    //
}
void User::user_log_out(){
    //unload user data
    unload_data();
    user_id="";
}
bool User::user_info_function(){
    while(1){
        cout<<"**************个人信息管理*****************"<<endl;
        cout<<"1.查看信息 2.修改信息 3.充值 4.返回用户界面"<<endl;
        cout<<"*******************************************"<<endl;
        cout<<" 请选择功能：";
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4");

        if(option=="1"){
            //seach user_info
            user_search_info();
            
        }
        else if(option=="2"){
            //update user_info
            if(user_change_info())return 1;
        }
        else if(option=="3"){
            //charge
            user_charge();
        }
        else if(option=="4"){
            //跳出个人信息界面，返回用户界面
            break;
        }
    }
    return 0;
}
void User::user_interface(){
    while(1){
        cout<<"********************用户功能********************"<<endl;
        cout<<"1、我是买家 2、我是卖家 3、个人信息 4、注销登录"<<endl;
        cout<<"*************************************************"<<endl;
        cout<<"请选择功能："<<endl;
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
            buyer_interface();
        }
        else if(option=="2"){
            //load seller data
            init_seller(user_id);
            //show seller functions
            seller_interface();
        }
        else if (option=="3"){
            //show info function
            if(user_info_function())break;
        }
        else if(option=="4"){
            //清除数据
            user_log_out();
            //跳出用户界面循环，返回主界面；
            break;
        }
    }
}
void User::user_register(){
    load_data();
    string user_id_pre="U";
    string uid=to_string(users+1);
    for(int i=0;i<int(3-uid.length());i++){
        user_id_pre+="0";
    }
    string new_uid=user_id_pre+uid;
    cout<<"你的账号是："<<new_uid<<endl;
    cout<<"请设置你的用户名："<<endl;
    string user_name;
    cin>>user_name;
    cout<<"请填写你的联系方式：";
    string phone;
    cin>>phone;
    cout<<"请填写你的地址：";
    string address;
    cin>>address;
    cout<<"请设置密码：";
    string passwd1;
    cin>>passwd1;
    cout<<"再次输入密码：";
    string passwd2;
    cin>>passwd2;
    if(passwd1==passwd2){
        string instr="INSERT INTO user VALUES ("+new_uid+","+user_name+","+passwd1+","+phone+","+address+")";
        get_sql(instr);
        show_data();
    }
    else{

    }
}
bool User::user_change_info(){
    cout<<"1. 联系方式 2.地址 3.密码"<<endl;
    cout<<"请选择你要修改的信息：";
    string option;
    do{
        cin>>option;
        if(option!="1"&&option!="2"&&option!="3"){
            cout<<"选项错误，要重新选择吗？(y/n)";
            string optionyn;
            do{
                cin>>optionyn;
                if(optionyn!="y"&&optionyn!="Y"&&optionyn!="n"&&optionyn!="N"){
                    cout<<"输入错误，请重新输入(y/n)"<<endl;
                }
            }while(optionyn!="y"&&optionyn!="Y"&&optionyn!="n"&&optionyn!="N");
            if(optionyn=="N"||optionyn=="n"){
                cout<<"放弃修改。"<<endl;
                break;
            }
        }

    }while(option!="1"&&option!="2"&&option!="3");
    if(option=="1"){
        cout<<"请输入修改后的联系方式：";
        string phone;
        cin>>phone;
        cout<<"请确认修改后的联系方式无误："<<endl;
        cout<<"联系方式："<<phone<<endl;
        cout<<"(y/n)";
        string option;
        cin>>option;
        if(option=="Y"||option=="y"){
            string instr = "UPDATE user SET 联系方式 = "+phone+" WHERE 用户ID = "+user_id;
            get_sql(instr);
        }

    }
    else if(option=="2"){
        cout<<"请输入修改后的地址：";
        string new_addr;
        cin>>new_addr;
        cout<<"请确认修改后的地址无误：";
        cout<<"新地址："<<new_addr<<endl;
        cout<<"(y/n)";
        string option;
        cin>>option;
        if(option=="Y"||option=="y"){
            string instr="UPDATE user SET 地址 = "+new_addr+" WHERE 用户ID = "+user_id;
            get_sql(instr);
        }
        
    }
    else if(option =="3"){
        cout<<"请输入修改后的密码：";
        string new_pw;
        cin>>new_pw;
        cout<<"请确认修改后的密码：";
        cout<<"新密码："<<new_pw<<endl;
        cout<<"(y/n)";
        string option;
        cin>>option;
        if(option=="Y"||option=="y"){
            string instr="UPDATE user SET 密码 = "+new_pw+" WHERE 用户ID = "+user_id;
            get_sql(instr);
        }
        cout<<"请重新登录！"<<endl;
        return 1;
    }
    return 0;
}
void User::user_charge(){
    cout<<"输入你想要充值的金额：";
    string charge_value;
    cin>>charge_value;
    //生成订单信息并入表,不需要时间
    string instr1="INSERT INTO charge VALUES ("+user_id+","+charge_value+")";
    get_sql(instr1);
    //更新余额信息
    Data::user_info*p=get_user(user_id);
    cout<<"old balance :"<<p->user_balance<<endl;
    float new_balance=stof(charge_value)+stof(p->user_balance);
    cout<<"new balance :"<<new_balance<<endl;
    string instr2="UPDATE user SET 钱包余额 = "+to_string(new_balance)+" WHERE 用户ID = "+user_id;
    get_sql(instr2);
}
void User::user_search_info(){
    string instr="SELECT * FROM user WHERE 用户ID = "+user_id;
    get_sql(instr);
}
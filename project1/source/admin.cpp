#include"../includes/admin.h"
void Admin::admin_log_in(){
    string in_account,in_password;
    bool flag=false;
    do{
        cout<<"输入管理员账号：";
        cin>>in_account;
        cout<<"输入管理员密码：";
        cin>>in_password;
        flag=in_account==ADMIN_ACCOUNT&&in_password==ADMIN_PASSWORD;
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

    //登录成功
    if(flag){
        cout<<"登录成功"<<endl;
        //加载管理员数据
        init_admin();
        //进入管理员界面
        admin_interface();
    }

}
void Admin::admin_show_product_list(){
    string instr="SELECT * FROM commodity";
    get_sql(instr);
}
void Admin::admin_search_product(){
    cout<<"请输入商品名称：";
    string product_name;
    cin>>product_name;
    string instr="SELECT * FROM commodity WHERE 名称 CONTAINS "+product_name;
    if(!get_sql(instr)){
        cout<<"没有找到商品"<<endl;
    }
}
void Admin::admin_remove_product(){
    string product_id;
    cout<<"输入商品ID:";
    cin>>product_id;
    string instr="UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = "+product_id;
    cout<<"instr:"<<instr;
    get_sql(instr);
    show_data();
}
void Admin::admin_show_history_orders(){
    string instr="SELECT * FROM order";
    get_sql(instr);
}
void Admin::admin_show_users_list(){
    string instr="SELECT * FROM user";
    get_sql(instr);
}
void Admin::admin_block_user(){
    //更改用户状态并下架用户商品
    string block_user_ID;
    cout<<"请输入要封禁的用户ID:";
    cin>>block_user_ID;
    cout<<"确认要封禁该用户吗？(y/n)"<<endl;
    string option;
    cin>>option;
    if(option=="y"){
        string commodity_sql_instr="UPDATE commodity SET 商品状态 = 已下架 WHERE 卖家ID = "+block_user_ID;
        string user_sql_instr="UPDATE user SET 用户状态 = 封禁 WHERE 用户ID = "+block_user_ID;
        get_sql(commodity_sql_instr);
        get_sql(user_sql_instr);
        show_data();
    }

}
void Admin::admin_interface(){
    while(1){
        cout<<"请选择功能："<<endl;
        cout<<"1.查看所有商品 2.搜索商品 3.下架商品 4.查看所有订单 5.查看所有用户 6.封禁用户 7.注销"<<endl;
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"&&option!="5"&&option!="6"&&option!="7"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4"&&option!="5"&&option!="6"&&option!="7");

        if(option=="1"){
            admin_show_product_list();
        }
        else if(option=="2"){
            admin_search_product();
        }
        else if(option=="3"){
            admin_remove_product();
        }
        else if(option=="4"){
            admin_show_history_orders();
        }
        else if(option=="5"){
            admin_show_users_list();
        }
        else if(option=="6"){
            admin_block_user();
        }
        else if(option=="7"){
            break;
        }
    }
}
void Admin::init_admin(){
    load_data();
    show_data();
    //cout<<"load success"<<endl;
    //show_data();
}
#include"../includes/buyer.h"
void Buyer::init_buyer(string user_id){
    buyer_id=user_id;
    load_buyer_data(buyer_id);
    show_data();
}
void Buyer::buyer_show_product_list(){
    //generate sql instr
    string instr="SELECT * FROM commodity";
    //send to data
    get_sql(instr);
}
void Buyer::buyer_search_product(){
    cout<<"请输入商品名称：";
    string product_name;
    cin>>product_name;
    string instr="SELECT * FROM commodity WHERE 名称 CONTAINS "+product_name;
    if(!get_sql(instr)){
        cout<<"没有找到商品"<<endl;
    }
}
void Buyer::buyer_history_orders(){
    string instr="SELECT * FROM order";
    if(!get_sql(instr)){
        cout<<"没有历史订单"<<endl;
    }
}
void Buyer::buyer_detail_info(){
    cout<<"输入商品ID：";
    string product_ID;
    cin>>product_ID;
    string instr="SELECT * FROM commodity WHERE ID CONTAINS "+product_ID;
    if(!get_sql(instr)){
        cout<<"没有找到该商品"<<endl;
    }
}
void Buyer::buy_product(){
    
}
void Buyer::buyer_interface(){
    while(1){
        cout<<"请选择功能："<<endl;
        cout<<"买家功能"<<endl;
        cout<<"1.查看商品列表 2.购买商品 3.搜索商品 4.查看历史订单 5.查看商品详细信息 6.返回用户主界面"<<endl;
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"&&
            option!="5"&&option!="6"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4"&&
            option!="5"&&option!="6");

        if(option=="1"){
            //product list
            buyer_show_product_list();
        }
        else if(option=="2"){
            //buy product
            buy_product();
        }
        else if(option=="3"){
            //search product
            buyer_search_product();
        }
        else if(option=="4"){
            //history order
            buyer_history_orders();
        }
        else if(option=="5"){
            //detail
            buyer_detail_info();
        }
        else if(option=="6"){
            //back to user interface
            unload_data();
            break;
        }
    }
}
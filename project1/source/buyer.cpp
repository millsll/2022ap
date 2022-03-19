#include"../includes/buyer.h"
#include<ctime>
#include<sstream>
#include<iomanip>
using namespace std;
void Buyer::init_buyer(string user_id){
    buyer_id=user_id;
    //show_data();
}
void Buyer::buyer_show_product_list(){
    //generate sql instr
    string instr="SELECT * FROM commodity WHERE 商品状态 = 销售中";
    //send to data
    get_sql(instr);
}
void Buyer::buyer_search_product(){
    cout<<"请输入商品名称：";
    string product_name;
    cin>>product_name;
    //模糊匹配
    string instr="SELECT * FROM commodity WHERE 名称 CONTAINS "+product_name;
    if(!get_sql(instr)){
        cout<<"没有找到商品"<<endl;
    }
}
void Buyer::buyer_history_orders(){
    string instr="SELECT * FROM order WHERE 买家ID = "+buyer_id;
    if(!get_sql(instr)){
        cout<<"没有历史订单"<<endl;
    }
}
void Buyer::buyer_detail_info(){
    cout<<"输入商品ID：";
    string product_ID;
    cin>>product_ID;
    string instr="SELECT * FROM commodity WHERE 商品ID CONTAINS "+product_ID;
    if(!get_sql(instr)){
        cout<<"没有找到该商品"<<endl;
    }
}
void Buyer::buy_product(){
    string product_id;
    string quantity;
    cout<<"请输入商品ID：";
    cin>>product_id;
    cout<<"请输入购买数量：";
    cin>>quantity;
    //直接查询商品列表：
    Data::product_info*p=get_commodity(product_id);
    
    if(p){
        if(p->quantity<stoi(quantity.c_str())){
            cout<<"购买失败，商品数量不足！"<<endl;
            return;
        }
        float price=stoi(quantity.c_str())*p->price;
        Data::user_info*b=get_user(buyer_id);
        Data::user_info*s=get_user(p->seller_ID);
        if(b->user_ID==s->user_ID){
            cout<<"无法购买自己的商品"<<endl;
            return;
        }
        if(b){
            if(price<stof(b->user_balance.c_str())){
                int new_quantity=p->quantity-stoi(quantity.c_str());
                string c_quantity=to_string(new_quantity);
                //减少商品数量
                string instr1="UPDATE commodity SET 数量 = "+c_quantity+" WHERE 商品ID = "+product_id;
                get_sql(instr1);
                //若商品数量为0，下架
                if(new_quantity==0){
                    string instr0="UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = "+product_id;
                    get_sql(instr0);
                }
                //增加卖家余额
                ostringstream c_num;
                c_num.precision(1);
                c_num.setf(ios::fixed);
                
                float new_seller_balance=stof(s->user_balance.c_str())+price;
                ostringstream c_sbalance;
                c_num<<new_seller_balance;
                string c_seller_balance=c_num.str();
                string instr2="UPDATE user SET 钱包余额 = "+c_seller_balance+" WHERE 用户ID = "+p->seller_ID;
                get_sql(instr2);
                //减少买家余额
                float new_buyer_balance=stof(b->user_balance.c_str())-price;
                c_num.str("");
                c_num<<new_buyer_balance;
                string c_buyer_balance=c_num.str();
                cout<<c_buyer_balance<<endl;
                string instr3="UPDATE user SET 钱包余额 = "+c_buyer_balance+" WHERE 用户ID = "+buyer_id;
                get_sql(instr3);
                //新建订单
                //show_data();
                string order_id=to_string(orders+1);
                string order_pre="T";
                for(int i=0;i<int(3-order_id.length());i++){
                    order_pre=order_pre+"0";
                }
                string c_order=order_pre+order_id;
                time_t now=time(0);
                tm*ltm=localtime(&now);
                string year,month,day;
                year=to_string(ltm->tm_year+1900);
                month=to_string(1+ltm->tm_mon);
                day=to_string(ltm->tm_mday);
                c_num.str("");
                c_num<<p->price;
                string value="("+c_order+","+p->product_ID+","+c_num.str()+","+quantity+","+year+"-"+month+"-"+day+","+s->user_ID+","+b->user_ID+")";
                cout<<value<<endl;
                get_sql("INSERT INTO order VALUES "+value);
                c_num.str("");
            }
            else{
                cout<<"余额不足"<<endl;
                return;
            }
        }
    }
    else{
        cout<<"没有该商品"<<endl;
        return;
    }
    show_data();
}
void Buyer::buyer_interface(){
    while(1){
        
        cout<<"*****************************************买家功能***************************************"<<endl;
        cout<<"1.查看商品列表 2.购买商品 3.搜索商品 4.查看历史订单 5.查看商品详细信息 6.返回用户主界面"<<endl;
        cout<<"****************************************************************************************"<<endl;
        cout<<"请选择功能："<<endl;
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
            //unload_data();
            break;
        }
    }
}
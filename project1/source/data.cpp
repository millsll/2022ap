#include"../includes/data.h"
#include<fstream>
#include<iostream>
#include<sstream>
bool Data::get_sql(string sql_instr){
    istringstream in(sql_instr);
    string instrs[20];
    string tmp;
    int cnt=0;
    //split instr
    while(getline(in,tmp,' ')){
        instrs[cnt]=tmp;
        cnt++;
    }
    int i=1;
    if(instrs[0]=="SELECT"){
        //get colums
        string colums[10];
        int tmpcnt=0;
        while(instrs[i]!="FROM"){
            colums[tmpcnt]=instrs[i];
            i++;
            tmpcnt++;
        }
        //jump from
        i++;
        //get tablename
        string tablename=instrs[i];
        //get conditon if exist
        if(i!=cnt){
            
        }
    }
    else if(instrs[0]=="INSERT"){

    }
    else if(instrs[0]=="UPDATE"){

    }
    else if(instrs[0]=="DELETE"){
        
    }
    return false;
}
bool Data::match_account(string account, string password){
    bool flag=false;
    fstream user_f;
    user_f.open("./data/user.txt",ios::in);
    if(!user_f){
        cout<<"打开失败"<<endl;
        exit(-1);
    }
    string file_line;
    user_f>>file_line;
    //cout<<file_line<<endl;
    while(!user_f.eof()){
        user_f>>file_line;
        //cout<<file_line<<endl;
        if(file_line!=""){
            istringstream in(file_line);
            string item[7];
            int i=0;
            string tmp;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                i++;
            }
            if(item[0]==account){
                if(item[2]==password){
                    //登录匹配成功
                    flag=true;
                    //加载用户信息；
                    user_info*p=new user_info;
                    p->user_ID=item[0];
                    p->user_name=item[1];
                    p->user_password=item[2];
                    p->user_phonenumber=item[3];
                    p->user_address=item[4];
                    p->user_balance=item[5];
                    if(item[6]=="正常")p->user_state=1;
                    else if(item[6]=="封禁")p->user_state=0;
                    else{
                        cout<<"用户状态信息异常"<<endl;
                        exit(-1);
                    }
                }
            }
        }
    }
    user_f.close();
    return  flag;
}
void Data::unload_data(){

}
void Data::load_seller_data(string user_id){
    //load seller product
    fstream commodity_f;
    commodity_f.open("./data/commodity.txt",ios::in);
    if(!commodity_f){
        cout<<"open commodity.txt failed"<<endl;
        exit(-1);
    }
    string file_line;
    commodity_f>>file_line;
    product_info* tail=NULL;
    while(!commodity_f.eof()){
        //get line from file
        commodity_f>>file_line;
        //split with ","
        if(file_line!=""){
            istringstream in(file_line);
            string item[8];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                i++;
            }
            //if seller is not the user load the product 
            if(item[5]!=user_id){
                product_info*p=new product_info;
                p->next=NULL;
                p->product_ID=item[0];
                p->product_name=item[1];
                p->price=atof(item[2].c_str());
                p->quantity=atoi(item[3].c_str());
                p->discription=item[4];
                p->seller_ID=item[5];
                p->time=item[6];
                if(item[7]=="销售中")
                    p->product_state=1;
                else if(item[7]=="已下架")
                    p->product_state=0;
                else{
                    cout<<"商品状态信息错误"<<endl;
                    exit(-1);
                }
                //插入商品表
                if(product_table==NULL)product_table=p;
                else tail->next=p;
                tail=p;
            }
            
        }
    }
    commodity_f.close();
    //load seller orders
    fstream orders_f;
    orders_f.open("./data/order.txt",ios::in);
    if(!orders_f){
        cout<<"open order.txt failed"<<endl;
        exit(-1);
    }
    //string file_line;
    orders_f>>file_line;
    order_info* tail2=NULL;
    while(!orders_f.eof()){
        //getline from file
        orders_f>>file_line;
        if(file_line!=""){
            istringstream in(file_line);
            string item[7];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                i++;
            }
            if(item[5]==user_id){
                order_info*p=new order_info;
                p->next=NULL;
                p->order_ID=item[0];
                p->product_ID=item[1];
                p->price=atof(item[2].c_str());
                p->quantity=atoi(item[3].c_str());
                p->deal_time=item[4];
                p->seller_ID=item[5];
                p->buyer_ID=item[6];
                if(order_table==NULL)order_table=p;
                else tail2->next=p;
                tail2=p;
            }
        }
    }
    orders_f.close();

}
void Data::load_buyer_data(string user_id){
    //just like seller
    //load all product except own
    //load buyer orders
}
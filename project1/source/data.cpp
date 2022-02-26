#include"../includes/data.h"
#include<fstream>
#include<iostream>
#include<sstream>
Data::Data(){
    product_table=NULL;
    order_table=NULL;
    user_table=NULL;
    products=0;
    orders=0;
    users=0;
    sys_state=unlog;
}
Data::~Data(){
    if(product_table){
        product_info*p=product_table;
        while(p){
            product_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    if(order_table){
        order_info*p=order_table;
        while(p){
            order_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    if(user_table){
        user_info*p=user_table;
        while(p){
            user_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    product_table=NULL;
    order_table=NULL;
    user_table=NULL;
    products=0;
    orders=0;
    users=0;
    sys_state=unlog;
}
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
    //analsys
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
int Data::match_account(string account, string password){
    int flag=0;
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
                    flag=1;
                    //检查用户状态
                    if(item[6]=="正常"){
                        flag=2;
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
                        user_table=p;
                    }
                }
            }
        }
    }
    user_f.close();
    return  flag;
}
void Data::unload_data(){
    if(product_table){
        product_info*p=product_table;
        while(p){
            product_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    if(order_table){
        order_info*p=order_table;
        while(p){
            order_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    if(user_table){
        user_info*p=user_table;
        while(p){
            user_info*tmp=p;
            p=p->next;
            delete tmp;
        }
    }
    product_table=NULL;
    order_table=NULL;
    user_table=NULL;
    products=0;
    orders=0;
    users=0;
    sys_state=unlog;
}
void Data::load_seller_data(string user_id){
    sys_state=seller;
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
            //if seller is the user load the product 
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
    sys_state=buyer;
    //load all product except own
    fstream commodity_f;
    commodity_f.open("./data/commodity.txt",ios::in);
    if(!commodity_f){
        cout<<"open commodity.txt failed"<<endl;
        exit(-1);
    }
    string file_line;
    commodity_f>>file_line;
    product_info*tail=NULL;
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
            //检查是否销售中
            if(item[7]=="销售中"){
                product_info*p=new product_info;
                p->next=NULL;
                p->product_ID=item[0];
                p->product_name=item[1];
                p->price=atof(item[2].c_str());
                p->quantity=atoi(item[3].c_str());
                p->discription=item[4];
                p->seller_ID=item[5];
                p->time=item[6];
                p->product_state=1;
                if(product_table==NULL)product_table=p;
                else tail->next=p;
                tail=p;
            }
            
        }
    }
    commodity_f.close();
    //load buyer orders
    fstream orders_f;
    orders_f.open("./data/order.txt",ios::in);
    if(!orders_f){
        cout<<"open order.txt failed"<<endl;
        exit(-1);
    }
    order_info*tail2=NULL;
    while(!orders_f.eof()){
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
            if(item[6]==user_id){
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
void Data::load_admin_data(){
    cout<<"start load"<<endl;
    sys_state=admin;
    fstream commodity_f;
    commodity_f.open("./data/commodity.txt",ios::in);
    if(!commodity_f){
        cout<<"open commodity.txt failed"<<endl;
        exit(-1);
    }
    string file_line;
    commodity_f>>file_line;
    product_info*tail=NULL;
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
            if(product_table==NULL)product_table=p;
            else tail->next=p;
            tail=p;
        }
    }
    commodity_f.close();
    cout<<"load product success"<<endl;
    fstream orders_f;
    orders_f.open("./data/order.txt",ios::in);
    if(!orders_f){
        cout<<"open order.txt failed"<<endl;
        exit(-1);
    }
    order_info*tail2=NULL;
    orders_f>>file_line;
    while(!orders_f.eof()){
        orders_f>>file_line;
        if(file_line!=""){
            stringstream in(file_line);
            string item[7];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                i++;
            }
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
    orders_f.close();
    fstream users_f;
    users_f.open("./data/user.txt",ios::in);
    if(!users_f){
        cout<<"open user.txt failed"<<endl;
        exit(-1);
    }
    users_f>>file_line;
    user_info*tail3=NULL;
    while(!users_f.eof()){
        users_f>>file_line;
        if(file_line!=""){
            stringstream in(file_line);
            string item[7];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                i++;
            }
            user_info*p=new user_info;
            p->next=NULL;
            p->user_ID=item[0];
            p->user_name=item[1];
            p->user_password=item[2];
            p->user_phonenumber=item[3];
            p->user_address=item[4];
            p->user_balance=item[5];
            if(item[6]=="正常")p->user_state=1;
            else if(item[6]=="封禁")p->user_state=0;
            else{
                cout<<item[6]<<endl;
                cout<<"用户状态信息异常"<<endl;
                exit(-1);
            }
            if(user_table==NULL)user_table=p;
            else tail3->next=p;
            tail3=p;
        }
    }
    users_f.close();
}
void Data::show_data(){
    product_info*p=product_table;
    cout<<"product:"<<endl;
    while(p){
        cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->seller_ID<<'\t'<<p->price<<'\t'<<p->quantity<<'\t'<<p->time<<'\t'<<p->product_state<<endl;
        p=p->next;
    }
    order_info*p2=order_table;
    cout<<"order:"<<endl;
    while(p2){
        cout<<p2->order_ID<<'\t'<<p2->product_ID<<p2->buyer_ID<<'\t'<<p2->seller_ID<<'\t'<<p2->quantity<<'\t'<<p2->price<<'\t'<<p2->deal_time<<endl;
        p2=p2->next;
    }
    user_info*p3=user_table;
    cout<<"user:"<<endl;
    while(p3){
        cout<<p3->user_ID<<'\t'<<p3->user_name<<endl;
        p3=p3->next;
    }
}
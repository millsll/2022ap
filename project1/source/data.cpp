#include"../includes/data.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<ctime>
#include<iomanip>
Data::Data(){
    product_table=NULL;
    order_table=NULL;
    user_table=NULL;
    charge_table=NULL;
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
void Data::change_state(int state){
    switch (state)
    {
    case 1:
        sys_state=admin;
        break;
    case 2:
        sys_state=seller;
        break;
    case 3:
        sys_state=buyer;
        break;
    case 4:
        sys_state=unlog;
        break;
    default:
        cout<<"unkown state"<<endl;
        exit(-1);
        break;
    }
}
bool Data::get_sql(string sql_instr){
    cout<<sql_instr<<endl;
    istringstream in(sql_instr);
    string instrs[20];
    string tmp;
    int cnt=0;
    //split instr
    while(getline(in,tmp,' ')){
        instrs[cnt]=tmp;
        cnt++;
    }
    for(int i=0;i<cnt;i++)cout<<instrs[i]<<endl;
    //analsys
    int i=1;
    if(instrs[0]=="SELECT"){
        //get colums
        string colums[10];
        int tmpcnt=0;
        while(instrs[i]!="FROM"&&i<cnt){
            colums[tmpcnt]=instrs[i];
            i++;
            tmpcnt++;
        }
        //jump "from"
        i++;
        //get tablename
        string tablename=instrs[i];

        //get conditon if exist
        if(i<cnt-1){
            bool flag;
            if(instrs[6]=="=")flag=0;
            else if(instrs[6]=="CONTAINS")flag=1;
            string condition_colum=instrs[5];
            string condition_value=instrs[7];
            deal_select(tablename,condition_colum,condition_value,flag);
        }
        else{
            deal_select(tablename);
        }
    }
    else if(instrs[0]=="INSERT"){
        string tablename=instrs[2];
        string values=instrs[4];
        cout<<values<<endl;
        deal_insert(tablename,values);
        
    }
    else if(instrs[0]=="UPDATE"){
        string tablename=instrs[1];
        //index 2: SET
        //index 3~5: changes:
        string change_colum=instrs[3];
        string change_value=instrs[5];
        //index 6:WHERE
        //index 7~9:condition, suppose equal
        string condition_colum=instrs[7];
        string condition_value=instrs[9];
        deal_update(tablename,condition_colum,condition_value,change_colum,change_value);
    }
    else if(instrs[0]=="DELETE"){
        string tablename=instrs[2];
        string colum=instrs[4];
        string value=instrs[6];
        deal_delete(tablename,colum,value);
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
                        data_user_id=account;
                        //user_table=p;
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
void Data::load_charge(){
    fstream charge_f;
    charge_f.open("./data/charge.txt",ios::in);
    if(!charge_f){
        cout<<"open charge.txt failed"<<endl;
        exit(-1);
    }
    string file_line;
    charge_f>>file_line;
    cout<<file_line<<endl;
    charge_info*tail=NULL;
    while(!charge_f.eof()){
        charge_f>>file_line;
        cout<<file_line<<endl;
        charges++;
        if(file_line!=""){
            istringstream in(file_line);
            string item[8];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                cout<<item[i]<<endl;
                i++;
            }
            charge_info*p=new charge_info;
            p->next=NULL;
            p->charge_time=item[3];
            p->charge_user_id=item[1];
            p->charge_value=stoi(item[2].c_str());
            cout<<"init a new log"<<endl;
            if(charge_table==NULL){cout<<"init nullptr"<<endl;charge_table=p;cout<<"load first"<<endl;}
            else tail->next=p;
            tail=p;
        }
    }
    charge_f.close();
    cout<<"load charge succ"<<endl;
}
void Data::unload_charge(){
    //TODO
}
void Data::load_data(){
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
        products++;
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
        orders++;
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
    cout<<"load order succ"<<endl;
    fstream users_f;
    //if(user_table)cout<<"not null"<<endl;
    //else cout<<"null"<<endl;
    users_f.open("./data/user.txt",ios::in);
    if(!users_f){
        cout<<"open user.txt failed"<<endl;
        exit(-1);
    }
    users_f>>file_line;
    user_info*tail3=NULL;
    while(!users_f.eof()){
        users_f>>file_line;
        users++;
        cout<<file_line<<endl;
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
void Data::load_user_data(string user_id){
    data_user_id==user_id;
    load_data();
    load_charge();
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
        cout<<p3->user_ID<<'\t'<<p3->user_name<<'\t'<<p3->user_state<<'\t'<<p3->user_balance<<endl;
        p3=p3->next;
    }
}
bool Data::deal_select(string table){
    cout<<"start deal select"<<endl;
    if(sys_state==admin){
        //show all information
        if(table=="commodity"){
            if(product_table){
                cout<<"商品ID\t名称\t价格\t上架时间\t卖家ID\t数量\t商品状态\n";
                product_info*p=product_table;
                while(p){
                    cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t'<<p->seller_ID<<'\t'<<p->quantity<<'\t';
                    if(p->product_state)cout<<"销售中"<<endl;
                    else cout<<"已下架"<<endl;
                    p=p->next;
                }
            }
            else{
                cout<<"暂无商品"<<endl;
            }
        }
        else if(table=="order"){
            if(order_table){
                cout<<"订单ID\t商品ID\t交易单价\t数量\t交易时间\t卖家ID\t买家ID\n";
                order_info*p=order_table;
                while(p){
                    cout<<p->order_ID<<'\t'<<p->product_ID<<'\t'<<p->price<<"\t\t"<<p->quantity<<'\t'<<p->deal_time<<'\t'<<p->seller_ID<<'\t'<<p->buyer_ID<<endl;
                    p=p->next;
                }
            }
            else{
                cout<<"暂无订单"<<endl;
            }
        }
        else if(table=="user"){
            if(user_table){
                cout<<"用户ID\t用户名\t联系方式\t地址\t\t钱包余额\t用户状态\n";
                user_info*p=user_table;
                while(p){
                    cout<<p->user_ID<<'\t'<<p->user_name<<'\t'<<p->user_phonenumber<<'\t'<<p->user_address<<"\t\t"<<p->user_balance<<'\t';
                    if(p->user_state)cout<<"正常"<<endl;
                    else cout<<"封禁"<<endl;
                    p=p->next;
                }
            }
            else {
                cout<<"暂无用户"<<endl;
            }
        }
    }
    else if(sys_state==seller){
        if(table=="commodity"){

        }
        else if(table=="order"){

        }
    }
    else if(sys_state==buyer){
        if(table=="commodity"){

        }
        else if(table=="order"){

        }
    }
    return false;
}
bool Data::deal_select(string table,string condition){
    return false;
}
bool Data::deal_delete(string table, string colum, string value){
    return false;
}
bool Data::deal_insert(string table,string values){
    string true_value=values.substr(1,values.length()-2);
        istringstream in(true_value);
        string true_values[10];
        string tmp;
        int cnt=0;
        while(getline(in,tmp,',')){
            true_values[cnt]=tmp;
            cout<<tmp<<endl;
            cnt++;
        }
        if(table=="commodity"){
        products++;
        product_info*p=new product_info;
        string tmp_id=to_string(products);
        string tmp_pre="M";
        for(int i=0;i<3-int(tmp_id.length());i++){
            tmp_pre=tmp_pre+"0";
        }
        p->product_ID=tmp_pre+tmp_id;
        p->price=stof(true_values[1].c_str());
        p->product_name=true_values[0];;
        p->product_state=1;
        p->quantity=stoi(true_values[2].c_str());
        p->seller_ID=data_user_id;
        time_t now=time(0);
        tm*ltm=localtime(&now);
        string year,month,day;
        year=to_string(ltm->tm_year+1900);
        month=to_string(1+ltm->tm_mon);
        day=to_string(ltm->tm_mday);
        p->time=year+"-"+month+"-"+day;
        p->next=NULL;
        insert_newdata(p);
    }
    else if(table=="order"){
        orders++;
        order_info*p=new order_info;
        p->order_ID=true_values[0];
        p->product_ID=true_values[1];
        p->price=stof(true_values[2].c_str());
        p->quantity=stoi(true_values[3].c_str());
        p->deal_time=true_values[4];
        p->seller_ID=true_values[5];
        p->buyer_ID=true_values[6];
        p->next=NULL;
        insert_newdata(p);
    }
    else if(table=="user"){
        users++;
        user_info*p=new user_info;
        p->user_ID=true_values[0];
        p->user_name=true_values[1];
        p->user_password=true_values[2];
        p->user_phonenumber=true_values[3];
        p->user_address=true_values[4];
        p->user_balance="0.0";
        p->user_state=1;
        p->next=NULL;
        insert_newdata(p);
    }
    return false;
}
void Data::insert_newdata(product_info*p){
    if(!product_table)product_table=p;
    else{
        product_info*q=product_table;
        while(q->next){
            //cout<<q->product_ID<<endl;
            q=q->next;
        }
        //cout<<q->product_ID<<endl;
        //cout<<p->seller_ID<<endl;
        q->next=p;
    }
}
void Data::insert_newdata(order_info*p){
    if(!order_table)order_table=p;
    else{
        order_info*q=order_table;
        while(q->next){
            q=q->next;
        }
        q->next=p;
    }
}
void Data::insert_newdata(user_info*p){
    if(!user_table)user_table=p;
    else{
        user_info*q=user_table;
        while(q->next){
            q=q->next;
        }
        q->next=p;
    }
}
bool Data::cmp_condition(user_info* p,string condition_c,string condition_v){
    if(condition_c=="用户ID"){
        return p->user_ID==condition_v;
    }
    return false;

}
bool Data::cmp_condition(product_info*p,string condition_c,string condition_v){
    if(condition_c=="卖家ID"){
        return p->seller_ID==condition_v;
    }
    else if(condition_c=="商品ID"){
        return p->product_ID==condition_v;
    }
    else if(condition_c=="商品状态"){
        return p->product_state;
    }
    return false;
}
bool Data::cmp_condition(order_info*p,string condition_c,string condition_v){
    if(condition_c=="卖家ID"){
        return p->seller_ID==condition_v;
    }
    else if(condition_c=="买家ID"){
        return p->buyer_ID==condition_v;
    }
    return false;
}
bool Data::cmp_condition(charge_info*p,string condition_c,string condition_v){
    if(condition_c=="用户ID"){
        return p->charge_user_id==condition_v;
    }
    return false;
}
void Data::change_value(user_info*p,string change_c,string change_v){
    if(change_c=="用户状态"){
        if(change_v=="封禁"){p->user_state=0; return;}
    }
    else if(change_c=="钱包余额"){
        p->user_balance=change_v;
    }
}
void Data::change_value(product_info*p,string change_c,string change_v){
    if(change_c=="商品状态"){
        if(change_v=="已下架"){
            p->product_state=0;
            return;
        }
        return;
    }
    if(change_c=="数量"){
        p->quantity=stoi(change_v.c_str());
        return;
    }
    if(change_c=="价格"){
        p->price=stof(change_v.c_str());
        return ;
    }
    if(change_c=="描述"){
        p->discription=change_v;
        return ;
    }
}
bool Data::deal_update(string table,string condition_c,string condition_v,string change_c,string change_v){
    cout<<"start deal update"<<endl;
    cout<<condition_c<<'\t'<<condition_v<<endl;
    cout<<change_c<<'\t'<<change_v<<endl;
    if(table=="user"){
        if(user_table){
            user_info*p=user_table;
            while(p){
                if(cmp_condition(p,condition_c,condition_v)){
                    change_value(p,change_c,change_v);
                }
                p=p->next;
            }
        }
    }
    else if(table=="commodity"){
        if(product_table){
            product_info*p=product_table;
            while(p){
                if(cmp_condition(p,condition_c,condition_v)){
                    change_value(p,change_c,change_v);
                }
                p=p->next;
            }
        }
    }
    return false;
}
bool Data::contain_condition(product_info*p,string condition_c,string condition_v){
    if(condition_c=="名称"){
        if(p->product_name.find(condition_v)==string::npos)return false;
        else return true;
    }
    else if(condition_c=="商品ID"){
        if(p->product_ID.find(condition_v)==string::npos)return false;
        else return true;
    }
    return  false;
}
bool Data::contain_condition(order_info*p,string condition_c,string condition_v){
    if(condition_c=="卖家ID"){
        if(p->seller_ID.find(condition_v))return false;
        else return true;
    }
    return false;
}
bool Data::deal_select(string table,string condition_c,string condition_v,bool condition){
    if(table=="commodity"){
        if(product_table){
            //判断是否有
            bool flag=false;
            product_info*p=product_table;
            while(p){
                if(condition){
                    if(contain_condition(p,condition_c,condition_v)){
                        flag=true;
                        break;
                    }
                }
                else{
                    if(cmp_condition(p,condition_c,condition_v)){
                        flag=true;
                        break;
                    }
                }
                p=p->next;
            }
            //输出结果
            p=product_table;
            if(flag){
                if((condition&&condition_c!="商品ID")||!condition){
                    cout<<"商品ID\t名称\t价格\t上架时间";
                    if(condition_c!="卖家ID")cout<<"\t卖家ID";
                    cout<<"\t数量";
                    if(condition_c!="商品状态")cout<<"\t商品状态";
                    cout<<endl;
                }
                while(p){
                    if(condition){
                        if(contain_condition(p,condition_c,condition_v)){
                            if(condition_c!="商品ID"){
                            cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t'<<p->seller_ID<<'\t'<<p->quantity<<'\t';
                            if(p->product_state)cout<<"销售中";
                            else cout<<"已下架";
                            cout<<endl;
                            }
                            else{
                                cout<<"商品ID："<<p->product_ID<<endl;
                                cout<<"商品名称："<<p->product_name<<endl;
                                cout<<"商品价格：";
                                printf("%.1f\n",p->price);
                                cout<<"上架时间："<<p->time<<endl;
                                cout<<"商品描述："<<p->discription<<endl;
                                cout<<"商品卖家："<<p->seller_ID<<endl;
                            }
                        }
                    }
                    else{
                        if(cmp_condition(p,condition_c,condition_v)){
                            cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t';
                            if(condition_c!="卖家ID")cout<<p->seller_ID<<'\t';
                            cout<<p->quantity;
                            if(condition_c!="商品状态"){
                                if(p->product_state)cout<<"\t销售中";
                                else cout<<"\t已下架";
                            }
                            cout<<endl;
                        }
                    }
                    p=p->next;
                }
            }
            else{

            }
        }
    }
    else if(table=="order"){
        if(order_table){
            //判断是否存在
            bool flag=false;
            order_info*p=order_table;
            while(p){
                if(condition){
                    if(contain_condition(p,condition_c,condition_v)){
                        flag=true;
                        break;
                    }
                }
                else{
                    if(cmp_condition(p,condition_c,condition_v)){
                        flag=true;
                        break;
                    }
                }
                p=p->next;
            }
            //输出结果
            p=order_table;
            if(flag){
                cout<<"订单ID\t商品ID\t交易单价\t数量\t交易时间\t";
                if(condition_c!="卖家ID")cout<<"卖家ID\t";
                cout<<"买家ID"<<endl;
                while(p){
                    if(condition){

                    }
                    else{
                        if(cmp_condition(p,condition_c,condition_v)){
                            cout<<p->order_ID<<'\t'<<p->product_ID<<'\t'<<p->price<<'\t'<<p->quantity<<'\t'<<p->deal_time<<'\t';
                            if(condition_c!="卖家ID")cout<<p->seller_ID<<'\t';
                            cout<<p->buyer_ID<<endl;
                        }
                    }
                    p=p->next;
                }
            }
        }
    }
    else if(table=="user"){
        if(user_table){
            user_info*p=user_table;
            while(p){
                if(!condition){
                    if(cmp_condition(p,condition_c,condition_v)){
                        //输出用户信息
                        cout<<"用户ID："<<p->user_ID<<endl;
                        cout<<"用户名："<<p->user_name<<endl;
                        cout<<"联系方式："<<p->user_phonenumber<<endl;
                        cout<<"地址："<<p->user_address<<endl;
                        //计算余额
                        //获取充值记录
                        string exp=generate_expression(condition_v);
                        trans(exp);
                        //获取出售金额
                        //获取购买金额
                    }
                }
                p=p->next;
            }
        }
    }
    return false;
}
void Data::write_back(){

}
Data::product_info*Data::get_commodity(string p_id){
    product_info*p=product_table;
    while(p){
        if(p->product_ID==p_id)return p;
        p=p->next;
    }
    return NULL;
}
Data::user_info*Data::get_user(string u_id){
    user_info*u=user_table;
    while(u){
        if(u->user_ID==u_id){
            return u;
        }
        u=u->next;
    }
    return NULL;
}
string Data::generate_expression(string uid){
    //初始化一个表达式
    Item*exp=NULL;
    //获取充值记录
    if(charge_table){
        cout<<"get charge log"<<endl;
        charge_info*p=charge_table;
        while(p){
            cout<<p->charge_value<<endl;
                if(cmp_condition(p,string("用户ID"),uid)){
                Item_num*num=new Item_num;
                num->num=p->charge_value;
                num->next=NULL;
                insert_item(1,num,exp);
            }
            p=p->next;
        }
    }
    //获取购买和出售记录
    if(order_table){
        order_info*p=order_table;
        cout<<"get order log"<<endl;
        while(p){
            if(cmp_condition(p,string("卖家ID"),uid)){
                cout<<"卖出："<<p->quantity<<","<<p->price<<endl;
                Item_num*num=new Item_num;
                num->num=p->price;
                num->next=NULL;
                insert_item(p->quantity,num,exp);
            }
            else if(cmp_condition(p,string("买家ID"),uid)){
                cout<<"花费："<<p->quantity<<","<<p->price<<endl;
                Item_num*num=new Item_num;
                num->num=-p->price;
                num->next=NULL;
                insert_item(p->quantity,num,exp);
            }
            p=p->next;
        }
    }
    cout<<"search data succ"<<endl;
    if(exp)cout<<"not null"<<endl;
    else cout<<"null"<<endl;
    Item*tmp=exp;
    while(tmp){
        cout<<tmp->factor<<":"<<endl;
        Item_num*x=tmp->nums;
        while(x){
            cout<<x->num<<" ";
            x=x->next;
        }
        cout<<endl;
        tmp=tmp->next;
    }
    Item*p=exp;
    string expression="";
    //取得每一项
    while(p){
        //取得每一项的数
        string c_item;
        if(p->factor==1){
            //不加括号
            c_item="";
            Item_num*pn=p->nums;
            char*t=(char*)malloc(100);
            sprintf(t,"%.1f",pn->num);
            c_item=c_item+t;
            delete t;
            pn=pn->next;
            while(pn){
                char*tmp=(char*)malloc(100);
                sprintf(tmp,"%.1f",pn->num);
                if(pn->num>=0)c_item=c_item+"+"+tmp;
                else c_item=c_item+tmp;
                delete tmp;
                pn=pn->next;
            }

        }
        else{
            //加括号
            c_item=to_string(p->factor)+"*(";
            Item_num*pn=p->nums;
            char*t=(char*)malloc(100);
            sprintf(t,"%.1f",pn->num);
            c_item=c_item+t;
            delete t;
            pn=pn->next;
            while(pn){
                char*tmp=(char*)malloc(100);
                sprintf(tmp,"%1.f",pn->num);
                if(pn->num>=0)c_item=c_item+"+"+tmp;
                else c_item=c_item+tmp;
                pn=pn->next;
                delete tmp;
            }
            c_item+=")";
        }
        if(expression==""){
            expression+=c_item;
        }
        else{
            expression=expression+"+"+c_item;
        }
        
        p=p->next;
    }
    cout<<expression<<endl;
    //释放空间
    Item*dp=exp;
    Item*dq=NULL;
    while(dp){
        dq=dp;
        dp=dp->next;
        Item_num*dpn=dq->nums;
        Item_num*dqn=NULL;
        while(dpn){
            dqn=dpn;
            dpn=dpn->next;
            delete dqn;
        }
        delete dq;
    }
    return expression;
}
void Data::insert_item(int factor, Item_num*num,Item*&exp){
    cout<<"insert factor:"<<factor<<"  num:"<<num->num<<endl;
    if(exp){
        Item*p=exp;
        Item*q=NULL;
        while(p){
            if(p->factor==factor){
                
                break;
            }
            q=p;
            p=p->next;
        }
        if(p){
            cout<<"exist equal factor:"<<p->factor<<endl;
            if(p->nums){
                cout<<"insert num:"<<num->num<<endl;
                Item_num*pn=p->nums;
                while(pn->next){
                    pn=pn->next;
                }
                pn->next=num;
            }
            else{
                p->nums=num;
            }
        }
        else{
            cout<<"not exist equal factor:"<<endl;
            cout<<"insert factor:"<<factor<<" num:"<<num->num<<endl;
            Item* new_item=new Item;
            new_item->factor=factor;
            new_item->next=NULL;
            new_item->nums=num;
            q->next=new_item;
        }
    }
    else{
        Item*new_item=new Item;
        new_item->factor=factor;
        new_item->next=NULL;
        new_item->nums=num;
        exp=new_item;
    }
}
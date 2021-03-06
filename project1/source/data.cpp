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
    message_table=NULL;
    products=0;
    orders=0;
    users=0;
    charges=0;
    messages=0;
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
}
int Data::get_sql(string sql_instr){
    add_commands(sql_instr);
    //cout<<sql_instr<<endl;
    istringstream in(sql_instr);
    string instrs[20];
    string tmp;
    int cnt=0;
    //split instr
    while(getline(in,tmp,' ')){
        instrs[cnt]=tmp;
        cnt++;
    }
    //for(int i=0;i<cnt;i++)cout<<instrs[i]<<endl;
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
            if(instrs[1]=="*"){
                deal_select(tablename,condition_colum,condition_value,flag);
            }
            else if(instrs[1]=="count(*)"){
                return deal_select_count(tablename,condition_colum,condition_value,flag);
            }
        }
        else{
            deal_select(tablename);
        }
    }
    else if(instrs[0]=="INSERT"){
        string tablename=instrs[2];
        string values=instrs[4];
        //cout<<values<<endl;
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
    
    return 0;
}
int Data::match_account(string account, string password){
    int flag=0;
    fstream user_f;
    user_f.open("./data/user.txt",ios::in);
    if(!user_f){
        cout<<"????????????"<<endl;
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
                    //??????????????????
                    flag=1;
                    //??????????????????
                    if(item[6]=="??????"){
                        flag=2;
                        //?????????????????????
                        user_info*p=new user_info;
                        p->user_ID=item[0];
                        p->user_name=item[1];
                        p->user_password=item[2];
                        p->user_phonenumber=item[3];
                        p->user_address=item[4];
                        p->user_balance=item[5];
                        if(item[6]=="??????")p->user_state=1;
                        else if(item[6]=="??????")p->user_state=0;
                        else{
                            cout<<"????????????????????????"<<endl;
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
    //cout<<file_line<<endl;
    charge_info*tail=NULL;
    while(!charge_f.eof()){
        charge_f>>file_line;
        //cout<<file_line<<endl;
        charges++;
        //cout<<"charge:"<<charges<<endl;
        if(file_line!=""){
            istringstream in(file_line);
            string item[8];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                //cout<<item[i]<<endl;
                i++;
            }
            charge_info*p=new charge_info;
            p->next=NULL;
            p->charge_time=item[3];
            p->charge_user_id=item[1];
            p->charge_value=stoi(item[2].c_str());
            p->charge_id=item[0];
            //cout<<"init a new log"<<endl;
            if(charge_table==NULL){charge_table=p;}
            else tail->next=p;
            tail=p;
        }
    }
    charge_f.close();
    //cout<<"load charge succ"<<endl;
}
void Data::unload_charge(){
    //TODO
    if(charge_table){
        charge_info*p=charge_table;
        while(p){
            charge_info*tmp=p;
            p=p->next;
            delete tmp;
        }
        charges=0;
        charge_table=NULL;
    }
}
void Data::load_message(){
    fstream msg_f;
    msg_f.open("./data/message.txt",ios::in);
    if(!msg_f){
        cout<<"open message failed"<<endl;
    }
    string file_line;
    msg_f>>file_line;
    //cout<<file_line<<endl;
    message_info*tail=NULL;
    while(!msg_f.eof()){
        msg_f>>file_line;
        messages++;
        if(file_line!=""){
            istringstream in(file_line);
            string item[8];
            string tmp;
            int i=0;
            while(getline(in,tmp,',')){
                item[i]=tmp;
                //cout<<tmp<<endl;
                i++;
            }
            message_info*p=new message_info;
            p->next=NULL;
            p->msg_id=item[0];
            p->sender_ID=item[1];
            p->recv_Id=item[2];
            if(item[3]=="??????")p->read=1;
            else if(item[3]=="??????")p->read=0;
            else{
                cout<<"??????????????????"<<endl;
            }
            p->content=item[4];
            if(message_table==NULL)message_table=p;
            else tail->next=p;
            tail=p;
        }
    }
    msg_f.close();
}
void Data::unload_message(){
    //TODO
    if(message_table){
        message_info*p=message_table;
        while(p){
            message_info*tmp=p;
            p=p->next;
            delete tmp;
        }
        messages=0;
        message_table=NULL;
    }
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
            if(item[7]=="?????????")
                p->product_state=1;
            else if(item[7]=="?????????")
                p->product_state=0;
            else{
                cout<<"????????????????????????"<<endl;
                cout<<item[7]<<endl;
                exit(-1);
            }
            if(product_table==NULL)product_table=p;
            else tail->next=p;
            tail=p;
        }
    }
    commodity_f.close();
    //cout<<"load product success"<<endl;
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
    //cout<<"load order succ"<<endl;
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
        //cout<<file_line<<endl;
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
            if(item[6]=="??????")p->user_state=1;
            else if(item[6]=="??????")p->user_state=0;
            else{
                cout<<item[6]<<endl;
                cout<<"????????????????????????"<<endl;
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
    load_message();
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
        //show all information
    if(table=="commodity"){
            cout<<"*******************************************************"<<endl;
        if(product_table){
            cout<<"??????ID\t??????\t??????\t????????????\t??????ID\t??????\t????????????\n";
            product_info*p=product_table;
            while(p){
                cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t'<<p->seller_ID<<'\t'<<p->quantity<<'\t';
                if(p->product_state)cout<<"?????????"<<endl;
                else cout<<"?????????"<<endl;
                p=p->next;
            }
        }
        else{
            cout<<"????????????"<<endl;
        }
            cout<<"*********************************************************"<<endl;
    }
    else if(table=="order"){
        if(order_table){
            cout<<"??????ID\t??????ID\t????????????\t??????\t????????????\t??????ID\t??????ID\n";
            order_info*p=order_table;
            while(p){
                cout<<p->order_ID<<'\t'<<p->product_ID<<'\t'<<p->price<<"\t\t"<<p->quantity<<'\t'<<p->deal_time<<'\t'<<p->seller_ID<<'\t'<<p->buyer_ID<<endl;
                p=p->next;
            }
        }
        else{
            cout<<"????????????"<<endl;
        }
    }
    else if(table=="user"){
        if(user_table){
            cout<<"??????ID\t?????????\t????????????\t??????\t\t????????????\t????????????\n";
            user_info*p=user_table;
            while(p){
                cout<<p->user_ID<<'\t'<<p->user_name<<'\t'<<p->user_phonenumber<<'\t'<<p->user_address<<"\t\t"<<p->user_balance<<'\t';
                if(p->user_state)cout<<"??????"<<endl;
                else cout<<"??????"<<endl;
                p=p->next;
            }
        }
        else {
            cout<<"????????????"<<endl;
        }
    }
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
            //cout<<tmp<<endl;
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
        p->discription=true_values[3];
        p->time=generate_time();
        p->next=NULL;
        insert_newdata(p);
        add_commodity(p);
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
        add_order(p);
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
        add_user(p);
    }
    else if(table=="charge"){
        //??????ID?????????ID???????????????
        charges++;
        charge_info*p=new charge_info;
        p->charge_user_id=true_values[0];
        p->charge_value=stof(true_values[1].c_str());
        cout<<"charge id:"<<charges<<endl;
        p->charge_id=generate_charge_id();
        cout<<"s_chargeid:"<<p->charge_id<<endl;
        //????????????
        p->charge_time=generate_time();
        insert_newdata(p);
        add_charge(p);
    }
    else if(table=="message"){
        messages++;
        message_info*p=new message_info;
        p->sender_ID=true_values[0];
        p->recv_Id=true_values[1];
        p->read=0;
        p->msg_id=generate_message_id();
        p->content=true_values[2];
        insert_newdata(p);
        add_message(p);
    }
    return false;
}
void Data::insert_newdata(charge_info*p){
    if(!charge_table)charge_table=p;
    else{
        charge_info*q=charge_table;
        while(q->next){
            q=q->next;
        }
        q->next=p;
    }
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
void Data::insert_newdata(message_info*p){
    if(!message_table)message_table=p;
    else{
        message_info*q=message_table;
        while(q->next){
            q=q->next;
        }
        q->next=p;
    }
}
bool Data::cmp_condition(user_info* p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        return p->user_ID==condition_v;
    }
    return false;

}
bool Data::cmp_condition(product_info*p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        return p->seller_ID==condition_v;
    }
    else if(condition_c=="??????ID"){
        return p->product_ID==condition_v;
    }
    else if(condition_c=="????????????"){
        return p->product_state;
    }
    return false;
}
bool Data::cmp_condition(order_info*p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        return p->seller_ID==condition_v;
    }
    else if(condition_c=="??????ID"){
        return p->buyer_ID==condition_v;
    }
    return false;
}
bool Data::cmp_condition(charge_info*p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        return p->charge_user_id==condition_v;
    }
    return false;
}
bool Data::cmp_condition(message_info*p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        return p->recv_Id==condition_v;
    }
    if(condition_c=="??????ID"){
        return p->sender_ID==condition_v;
    }
    if(condition_c=="??????ID"){
        return p->msg_id==condition_v;
    }
    return false;
}
void Data::change_value(user_info*p,string change_c,string change_v){
    if(change_c=="????????????"){
        if(change_v=="??????"){p->user_state=0; return;}
    }
    else if(change_c=="????????????"){
        float tmp=stof(change_v);
        //cout<<"tmp:"<<tmp<<endl;
        string s_v=float_to_s(tmp);
        //cout<<"s:"<<s_v<<endl;
        p->user_balance=s_v;
    }
    else if(change_c=="????????????"){
        p->user_phonenumber=change_v;
    }
    else if(change_c=="??????"){
        p->user_address=change_v;
    }
    else if(change_c=="??????"){
        p->user_password=change_v;
    }
}
void Data::change_value(product_info*p,string change_c,string change_v){
    if(change_c=="????????????"){
        if(change_v=="?????????"){
            p->product_state=0;
            return;
        }
        return;
    }
    if(change_c=="??????"){
        p->quantity=stoi(change_v.c_str());
        return;
    }
    if(change_c=="??????"){
        p->price=stof(change_v.c_str());
        return ;
    }
    if(change_c=="??????"){
        p->discription=change_v;
        return ;
    }
}
void Data::change_value(message_info*p,string change_c,string change_v){
    if(change_c=="?????????"){
        if(change_v=="??????"){
            p->read=1;
            return;
        }
    }
    return;
}
bool Data::deal_update(string table,string condition_c,string condition_v,string change_c,string change_v){
    //cout<<"start deal update"<<endl;
    //cout<<condition_c<<'\t'<<condition_v<<endl;
    //cout<<change_c<<'\t'<<change_v<<endl;
    if(table=="user"){
        if(user_table){
            user_info*p=user_table;
            while(p){
                if(cmp_condition(p,condition_c,condition_v)){
                    change_value(p,change_c,change_v);

                    break;
                }
                p=p->next;
            }
            //cout<<"p-balance:"<<p->user_balance<<endl;
            change_user();
        }
    }
    else if(table=="commodity"){
        if(product_table){
            product_info*p=product_table;
            while(p){
                if(cmp_condition(p,condition_c,condition_v)){
                    change_value(p,change_c,change_v);
                    break;
                }
                p=p->next;
            }
            change_commodity();
        }
    }
    else if(table=="message"){
        if(message_table){
            message_info*p=message_table;
            while(p){
                if(cmp_condition(p,condition_c,condition_v)){
                    change_value(p,change_c,change_v);
                    break;
                }
                p=p->next;
            }
            change_message();
        }
    }
    return false;
}
bool Data::contain_condition(product_info*p,string condition_c,string condition_v){
    if(condition_c=="??????"){
        if(p->product_name.find(condition_v)==string::npos)return false;
        else return true;
    }
    else if(condition_c=="??????ID"){
        if(p->product_ID.find(condition_v)==string::npos)return false;
        else return true;
    }
    return  false;
}
bool Data::contain_condition(order_info*p,string condition_c,string condition_v){
    if(condition_c=="??????ID"){
        if(p->seller_ID.find(condition_v))return false;
        else return true;
    }
    return false;
}
bool Data::deal_select(string table,string condition_c,string condition_v,bool condition){
    if(table=="commodity"){
        if(product_table){
            //???????????????
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
            //????????????
            p=product_table;
            if(flag){
                cout<<"*****************************************************************************************"<<endl;
                if((condition&&condition_c!="??????ID")||!condition){
                    cout<<"??????ID\t??????\t??????\t????????????";
                    if(condition_c!="??????ID")cout<<"\t??????ID";
                    cout<<"\t??????";
                    if(condition_c!="????????????")cout<<"\t????????????";
                    cout<<endl;
                }
                while(p){
                    if(condition){
                        if(contain_condition(p,condition_c,condition_v)){
                            if(condition_c!="??????ID"){
                            cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t'<<p->seller_ID<<'\t'<<p->quantity<<'\t';
                            if(p->product_state)cout<<"?????????";
                            else cout<<"?????????";
                            cout<<endl;
                            }
                            else{
                                cout<<"??????ID???"<<p->product_ID<<endl;
                                cout<<"???????????????"<<p->product_name<<endl;
                                cout<<"???????????????";
                                printf("%.1f\n",p->price);
                                cout<<"???????????????"<<p->time<<endl;
                                cout<<"???????????????"<<p->discription<<endl;
                                cout<<"???????????????"<<p->seller_ID<<endl;
                            }
                        }
                    }
                    else{
                        if(cmp_condition(p,condition_c,condition_v)){
                            cout<<p->product_ID<<'\t'<<p->product_name<<'\t'<<p->price<<'\t'<<p->time<<'\t';
                            if(condition_c!="??????ID")cout<<p->seller_ID<<'\t';
                            cout<<p->quantity;
                            if(condition_c!="????????????"){
                                if(p->product_state)cout<<"\t?????????";
                                else cout<<"\t?????????";
                            }
                            cout<<endl;
                        }
                    }
                    p=p->next;
                }
                cout<<"*****************************************************************************************"<<endl;
            }
            else{
                cout<<"?????????"<<endl;
            }
        }
    }
    else if(table=="order"){
        if(order_table){
            //??????????????????
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
            //????????????
            p=order_table;
            if(flag){
                cout<<"*******************************************************************************"<<endl;
                cout<<"??????ID\t??????ID\t????????????\t??????\t????????????\t";
                if(condition_c!="??????ID")cout<<"??????ID\t";
                cout<<"??????ID"<<endl;
                while(p){
                    if(condition){

                    }
                    else{
                        if(cmp_condition(p,condition_c,condition_v)){
                            cout<<p->order_ID<<'\t'<<p->product_ID<<'\t'<<p->price<<'\t'<<p->quantity<<'\t'<<p->deal_time<<'\t';
                            if(condition_c!="??????ID")cout<<p->seller_ID<<'\t';
                            cout<<p->buyer_ID<<endl;
                        }
                    }
                    p=p->next;
                }
                cout<<"*******************************************************************************"<<endl;
            }
            else{
                cout<<"?????????"<<endl;
            }
        }
    }
    else if(table=="user"){
        if(user_table){
            user_info*p=user_table;
            while(p){
                if(!condition){
                    if(cmp_condition(p,condition_c,condition_v)){
                        cout<<"**********************************"<<endl;
                        //??????????????????
                        cout<<"??????ID???"<<p->user_ID<<endl;
                        cout<<"????????????"<<p->user_name<<endl;
                        cout<<"???????????????"<<p->user_phonenumber<<endl;
                        cout<<"?????????"<<p->user_address<<endl;
                        cout<<"?????????";
                        //????????????
                        //??????????????????
                        string exp=generate_expression(condition_v);
                        //trans(exp);
                        float res=figure_exp(exp);
                        cout<<res<<endl;
                        //??????????????????
                        //??????????????????
                        cout<<"**********************************"<<endl;
                    }
                }
                p=p->next;
            }
        }
    }
    else if(table=="message"){
        bool flag=false;
        message_info*p=message_table;
        while(p){
            if(!condition){
                if(cmp_condition(p,condition_c,condition_v)){
                    flag=true;
                    break;
                }
            }
            p=p->next;
        }
        p=message_table;
        //cout<<condition_c<<" "<<condition_v<<endl;
        //cout<<p->sender_ID<<p->content<<endl;
        if(flag){
            while(p){
                if(!condition){
                    //cout<<"check msg"<<endl;
                    if(cmp_condition(p,condition_c,condition_v)){
                        cout<<p->sender_ID<<"\t"<<p->content<<"\t";
                        if(!p->read&&condition_c=="??????ID"){
                            cout<<"NEW???";
                            get_sql("UPDATE message SET ????????? = ?????? WHERE ??????ID = "+p->msg_id);
                        }
                        else if(condition_c=="??????ID"){
                            if(p->read)cout<<"??????";
                            else cout<<"??????";
                        }
                        cout<<endl;
                    }
                    else{
                        cout<<"cmp false"<<endl;
                    }
                }
                p=p->next;
            }
        }
        else{
            cout<<"?????????"<<endl;
        }
    }
    return false;
}
int Data::deal_select_count(string table,string condition_c,string condition_v,bool condition){
    int v_count=0;
    //cout<<"condition:"<<condition<<endl;
    //cout<<"?????????"<<message_table->read<<endl;
    //cout<<condition_c<<" "<<condition_v<<endl;
    if(table=="message"){
        message_info*p=message_table;
        while(p){
            if(!condition){
                if((!p->read)&&cmp_condition(p,condition_c,condition_v))v_count++;
            }
            p=p->next;
        }
        return v_count;
    }
    return v_count;
}

//????????????
void Data::add_charge(charge_info*p){
    fstream charge_f;
    charge_f.open("./data/charge.txt",ios::app);
    charge_f<<'\n'<<p->charge_id<<","<<p->charge_user_id<<","<<float_to_s(p->charge_value)<<","<<p->charge_time;

    charge_f.close();
}
void Data::add_order(order_info*p){
    fstream order_f;
    order_f.open("./data/order.txt",ios::app);
    order_f<<'\n'<<p->order_ID<<","<<p->product_ID<<","<<float_to_s(p->price)<<","<<p->quantity<<","<<p->deal_time<<","<<p->seller_ID<<","<<p->buyer_ID;
    order_f.close();
}
void Data::add_commodity(product_info*p){
    fstream commodity_f;
    commodity_f.open("./data/commodity.txt",ios::app);
    commodity_f<<'\n'<<p->product_ID<<","<<p->product_name<<","<<float_to_s(p->price)<<","<<p->quantity<<","<<p->discription<<","<<p->seller_ID<<","<<p->time<<",";
    if(p->product_state)commodity_f<<"?????????";
    else commodity_f<<"?????????";
    commodity_f.close();
}
void Data::add_user(user_info*p){
    fstream user_f;
    user_f.open("./data/user.txt");
    user_f<<'\n'<<p->user_ID<<","<<p->user_name<<","<<p->user_password<<","<<p->user_phonenumber<<","<<p->user_address<<","<<p->user_balance<<",";
    if(p->user_state)user_f<<"??????";
    else user_f<<"??????";
    user_f.close();
}
void Data::add_message(message_info*p){
    fstream message_f;
    message_f.open("./data/message.txt",ios::app);
    message_f<<"\n"<<p->msg_id<<","<<p->sender_ID<<","<<p->recv_Id<<",";
    if(p->read)message_f<<"??????"<<",";
    else message_f<<"??????"<<",";
    message_f<<p->content;
    message_f.close();
}
void Data::add_commands(string command){
    //TODO
    //get time
    fstream commands_f;
    commands_f.open("./data/commands.txt",ios::app);
    commands_f<<"\n"<<generate_time_s()<<": "<<command;
}
void Data::change_user(){
    fstream user_f;
    user_f.open("./data/user.txt",ios::trunc|ios::out);
    user_f<<"??????ID,?????????,??????,????????????,??????,????????????,????????????";
    user_info*p=user_table;
    while(p){
        user_f<<'\n'<<p->user_ID<<","<<p->user_name<<","<<p->user_password<<","<<p->user_phonenumber<<","<<p->user_address<<","<<p->user_balance<<",";
        if(p->user_state)user_f<<"??????";
        else user_f<<"??????";
        p=p->next;
    }
    user_f.close();
}
void Data::change_message(){
    fstream msg_f;
    msg_f.open("./data/message.txt",ios::trunc|ios::out);
    msg_f<<"??????ID,??????ID,??????ID,?????????,????????????";
    message_info*p=message_table;
    while(p){
        msg_f<<"\n"<<p->msg_id<<","<<p->sender_ID<<","<<p->recv_Id<<",";
        if(p->read)msg_f<<"??????"<<",";
        else msg_f<<"??????"<<",";
        msg_f<<p->content;
        p=p->next;
    }
    msg_f.close();
}
void Data::change_commodity(){
    //cout<<"modify commodity"<<endl;
    fstream commodity_f;
    commodity_f.open("./data/commodity.txt",ios::trunc|ios::out);
    commodity_f<<"??????ID,??????,??????,??????,??????,??????ID,????????????,????????????";
    product_info*p=product_table;
    while(p){
        commodity_f<<'\n'<<p->product_ID<<","<<p->product_name<<","<<float_to_s(p->price)<<","<<p->quantity<<","<<p->discription<<","<<p->seller_ID<<","<<p->time<<",";
        if(p->product_state)commodity_f<<"?????????";
        else commodity_f<<"?????????";
        p=p->next;
    }
    commodity_f.close();
}

//float to string ??????????????????
string Data::float_to_s(float f){
    ostringstream s_float;
    s_float.precision(1);
    s_float.setf(ios::fixed);
    s_float<<f;
    return s_float.str();
}
//?????????????????????
string Data::generate_commodity_id(){
    string suff=to_string(products);
    string pre="M";
    for(int i=0;i<3-int(suff.length());i++){
        pre+="0";
    }
    return pre+suff;
}
string Data::generate_order_id(){
    string suff=to_string(orders);
    string pre="T";
    for(int i=0;i<3-int(suff.length());i++){
        pre+="0";
    }
    return pre+suff;
}
string Data::generate_charge_id(){
    string suff=to_string(charges);
    string pre="C";
    for(int i=0;i<3-int(suff.length());i++){
        pre+="0";
    }
    return pre+suff;
}
string Data::generate_message_id(){
    string suff=to_string(messages);
    string pre="S";
    for(int i=0;i<3-int(suff.length());i++){
        pre+="0";
    }
    return pre+suff;
}
string Data::generate_time(){
    time_t now=time(0);
    tm*ltm=localtime(&now);
    string year,month,day;
    year=to_string(ltm->tm_year+1900);
    month=to_string(1+ltm->tm_mon);
    day=to_string(ltm->tm_mday);
    if(month.length()==1)month="0"+month;
    if(day.length()==1)day="0"+day;
    return year+"-"+month+"-"+day;
}
string Data::generate_time_s(){
    time_t now=time(0);
    tm*ltm=localtime(&now);
    string h,m,s;
    h=to_string(ltm->tm_hour);
    m=to_string(ltm->tm_min);
    s=to_string(ltm->tm_sec);
    return generate_time()+" "+h+":"+m+":"+s;
}
string Data::generate_user_id(){
    string suff=to_string(users);
    string pre="U";
    for(int i=0;i<3-int(suff.length());i++){
        pre+="0";
    }
    return pre+suff;
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
    //????????????????????????
    Item*exp=NULL;
    //??????????????????
    if(charge_table){
        //cout<<"get charge log"<<endl;
        charge_info*p=charge_table;
        while(p){
            //cout<<p->charge_value<<endl;
            if(cmp_condition(p,string("??????ID"),uid)){
                Item_num*num=new Item_num;
                num->num=p->charge_value;
                num->next=NULL;
                insert_item(1,num,exp);
            }
            p=p->next;
        }
    }
    //???????????????????????????
    if(order_table){
        order_info*p=order_table;
        //cout<<"get order log"<<endl;
        while(p){
            if(cmp_condition(p,string("??????ID"),uid)){
                //cout<<"?????????"<<p->quantity<<","<<p->price<<endl;
                Item_num*num=new Item_num;
                num->num=p->price;
                num->next=NULL;
                insert_item(p->quantity,num,exp);
            }
            else if(cmp_condition(p,string("??????ID"),uid)){
                //cout<<"?????????"<<p->quantity<<","<<p->price<<endl;
                Item_num*num=new Item_num;
                num->num=-p->price;
                num->next=NULL;
                insert_item(p->quantity,num,exp);
            }
            p=p->next;
        }
    }

    Item*p=exp;
    string expression="";
    //???????????????
    while(p){
        //?????????????????????
        string c_item;
        if(p->factor==1){
            //????????????
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
            //?????????
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
    //cout<<expression<<endl;
    //????????????
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
    //cout<<"insert factor:"<<factor<<"  num:"<<num->num<<endl;
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
            //cout<<"exist equal factor:"<<p->factor<<endl;
            if(p->nums){
                //cout<<"insert num:"<<num->num<<endl;
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
            //cout<<"not exist equal factor:"<<endl;
            //cout<<"insert factor:"<<factor<<" num:"<<num->num<<endl;
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
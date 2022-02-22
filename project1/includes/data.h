#ifndef DATA_HEAD
#define DATA_HEAD
#include<string>
#include<iostream>
using namespace std;
//数据查询
class Data{
    public:
    //接受sql指令并分析
    bool get_sql(string sql_instr);
    bool match_account(string account,string password);
    void init_user_info(string user_id);
    void load_seller_data(string uer_id);
    void load_buyer_data(string user_id);
    void unload_data();
    void load_admin_data();
    private:
    //用户信息数据结构
    struct user_info{
        //UXXX
        string user_ID;
        //10 chars
        string user_name;
        //less than 20 chars
        string user_password;
        //phone number less than 20 chars
        string user_phonenumber;
        //less than 20 chars
        string user_address;
        //余额
        string user_balance;
        user_info*next;
    }*user_table;
    int users;
    //商品信息数据结构
    struct product_info{
        //MXXX
        string product_ID;
        string product_name;
        float price;
        int quantity;
        string discription;
        string seller_ID;
        //yyyy-mm-dd
        string time;
        bool product_state;
        product_info*next;
    }*product_table;
    int products;
    //订单信息数据结构
    struct order_info{
        //TXXX
        string  order_ID;
        //MXXX
        string product_ID;
        //price
        float price;
        //quantity
        int quantity;
        //deal time 
        string deal_time;
        //seller
        string seller_ID;
        //buyer
        string buyer_ID;
        order_info*next;
    }*order_table;
    int orders;
    //充值记录数据结构
    int charges;
};


#endif
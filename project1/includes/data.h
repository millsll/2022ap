#ifndef DATA_HEAD
#define DATA_HEAD
#include<string>
#include<iostream>
#include"caculator.h"
using namespace std;
//数据查询
class Data:public Caculator
{
    public:
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
        bool user_state;
        user_info*next;
    };

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
    };

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
    };
    
    //充值记录数据维护
    struct charge_info{
        string charge_user_id;
        string charge_time;
        float charge_value;
        charge_info*next;
    };

    protected:
    int users;
    int products;
    int orders;
    int charges;
    private:
    string data_user_id;
    user_info*user_table;
    product_info*product_table;
    order_info*order_table;
    charge_info*charge_table;

    //item number
    struct Item_num{
        float num;
        Item_num*next;
    };
    //item struct
    struct Item{
        int factor;
        Item_num* nums;
        Item*next;
    };
    
    public:
    //接受sql指令并分析
    bool get_sql(string sql_instr);
    int match_account(string account,string password);
    void init_user_info(string user_id);
    void load_data();
    void load_charge();
    void unload_charge();
    void load_user_data(string user_id);
    void load_seller_data(string uer_id);
    void load_buyer_data(string user_id);
    void unload_data();
    void load_admin_data();
    void load_user_info(string user_id);
    void show_data();
    void change_state(int state);
    product_info*get_commodity(string p_id);
    user_info*get_user(string u_id);
    
    
    
    void write_back();
    Data();
    ~Data();
    private:
    enum state_type{admin,seller,buyer,unlog};
    state_type sys_state;

    bool deal_select(string table);
    bool deal_select(string table,string condition);
    bool deal_insert(string table,string values);
    bool deal_delete(string table,string colum,string value);
    bool deal_update(string table,string condition_c,string condition_v,string change_c,string change_v);
    bool cmp_condition(user_info*p,string condition_c,string condition_v);
    bool cmp_condition(product_info*p,string condition_c,string condition_v);
    bool cmp_condition(order_info*p,string condition_c,string condition_v);
    bool cmp_condition(charge_info*p,string condition_c,string condition_v);
    
    void change_value(product_info*p,string change_c,string change_v);
    void change_value(user_info*p,string change_c,string change_v);
    bool deal_select(string table,string condition_c,string condition_v,bool condition);
    bool contain_condition(product_info*p,string condition_c,string condition_v);
    bool contain_condition(order_info*p,string condition_c,string condition_v);
    void insert_newdata(product_info*p);
    void insert_newdata(order_info*p);
    void insert_newdata(user_info*p);
    string  generate_expression(string uid);
    void insert_item(int factor,Item_num*p,Item*&exp);
};


#endif
#ifndef DATA_HEAD
#define DATA_HEAD
#include<string>
#include<fstream>
using namespace std;
//数据查询
class Data{
    public:
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
    }*user_table;
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
    }*product_table;
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
        
    }*order_table;
};
#endif
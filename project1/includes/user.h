#include"seller.h"
#include"buyer.h"
#include"caculator.h"
//用户功能,继承卖家买家
class User: private Seller,private Buyer
{
    public:
    //用户登录
    void user_log_in();
    //用户注销
    void user_log_out();
    //用户功能界面
    void user_interface();
    //用户注册
    void user_register();
    //用户信息功能
    void user_info_function();
    private:
    string user_id;
};
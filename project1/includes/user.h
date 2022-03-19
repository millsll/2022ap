#include"seller.h"
#include"buyer.h"
#include"caculator.h"
//用户功能,继承卖家买家
class User: private Seller,private Buyer
{
    public:
    //用户登录
    void user_log_in();
        //用户注册
    void user_register();
    
    private:
    string user_id;
    //用户注销
    void user_log_out();
    //用户功能界面
    void user_interface();
    //用户信息功能
    bool user_info_function();
    //修改用户信息
    bool user_change_info();
    //用户充值
    void user_charge();
    //查看用户信息
    void user_search_info();
    //消息面板
    void user_message_interface();
    //查看留言
    void user_show_message();
    //查看已发送的留言
    void user_show_sent_message();
    //发送留言
    void user_send_message();
};
#include"data.h"
#define ADMIN_ACCOUNT "admin"
#define ADMIN_PASSWORD "admin"
//管理员功能
class Admin: public Data
{
    public:
    //管理员登录，写死账号密码
    void admin_log_in();
    void admin_interface();
    void init_admin();
    void admin_show_product_list();
    void admin_search_product();
    void admin_remove_product();
    void admin_show_history_orders();
    void admin_show_users_list();
    void admin_block_user();
    void admin_search_seller_order();
    void admin_search_buyer_order();
    //using Data::Data;
    private:
};
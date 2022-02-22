#include"data.h"
//卖家功能
class Seller:virtual public Data
{
    public:
    void init_seller(string user_id);
    void seller_interface();
    void seller_post_product();
    void seller_show_product_list();
    void seller_change_product();
    void seller_remove_product();
    void seller_history_order();
    private:
    string seller_id;
};
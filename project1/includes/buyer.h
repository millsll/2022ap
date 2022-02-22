#include"data.h"
//买家功能
class Buyer: virtual public Data
{
    public:
    void buyer_interface();
    void init_buyer(string user_id);
    void buyer_show_product_list();
    void buy_product();
    void buyer_search_product();
    void buyer_history_orders();
    void buyer_detail_info();
    private:
    string buyer_id;
};
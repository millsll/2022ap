#include"seller.h"
#include"buyer.h"
//用户功能,继承卖家买家
class User: public Seller,public Buyer
{
    public:
    //用户登录
    void user_log_in();
    private:
};
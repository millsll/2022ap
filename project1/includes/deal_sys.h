#include"admin.h"
#include"user.h"
//组合系统
class Deal_sys{
    public:
    //首界面登录功能
    void sys_log_in();
    //
    private:
    Admin admin;
    User user;
};
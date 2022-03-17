#include"../includes/seller.h"
void Seller::init_seller(string user_id){
    seller_id=user_id;
    show_data();
}
void Seller::seller_post_product(){
    string instr="INSERT INTO commodity VALUES";
    
    string p_name,p_price,p_quantity,p_discription;
    cout<<"请输入商品名称：";
    cin>>p_name;
    cout<<"请输入商品价格：";
    cin>>p_price;
    cout<<"请输入商品数量：";
    cin>>p_quantity;
    cout<<"请输入商品描述：";
    cin>>p_discription;
    cout<<"请确认发布商品信息无误："<<endl;
    cout<<"商品名称："<<p_name;
    cout<<"商品价格："<<p_price;
    cout<<"商品数量："<<p_quantity;
    cout<<"商品描述："<<p_discription;
    cout<<"你确认要发布商品吗？(y/n)";
    string option;
    do{
        cin>>option;
        if(option!="y"&&option!="Y"&&option!="n"&&option!="N"){
            cout<<"选项错误，请重新选择：";
        }
    }while(option!="Y"&&option!="y"&&option!="N"&&option!="n");
    if(option=="Y"||option=="y"){
        instr=instr+" ("+p_name+","+p_price+","+p_quantity+","+p_discription+")";
        get_sql(instr);
    }
    else{
        cout<<"返回上一级"<<endl;
    }
}
void Seller::seller_show_product_list(){
    string instr="SELECT * FROM commodity WHERE 卖家ID = "+ seller_id;
    get_sql(instr);
}
void Seller::seller_remove_product(){
    string product_id;
    cout<<"输入商品ID:";
    cin>>product_id;
    string instr="UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = "+product_id;
    get_sql(instr);
}
void Seller::seller_change_product(){
    string p_id,option;
    cout<<"输入要修改的商品编号："<<endl;
    cin>>p_id;
    cout<<"选择想要修改的商品属性（1.价格 2.描述 3.数量）：";
    do{
        cin>>option;
        if(option!="1"&&option!="2"&&option!="3"){
            cout<<"选项错误，请重新选择：";
        }
    }while(option!="1"&&option!="2"&&option!="3");
    if(option=="1"){
        cout<<"请输入修改后的商品价格：";
        string price;
        cin>>price;
        cout<<"请确认修改后的商品信息无误："<<endl;
        Data::product_info*p=get_commodity(p_id);
        cout<<"商品ID："<<p->product_ID<<endl;
        cout<<"商品名称："<<p->product_name<<endl;
        cout<<"商品价格："<<price;
        cout<<"商品描述："<<p->discription<<endl;
        string instr="UPDATE commodity SET 价格 = "+price+" WHERE 商品ID = "+p_id;
        get_sql(instr);
    }
    else if(option=="2"){
        cout<<"请输入修改后的商品你描述：";
        string discription;
        cin>>discription;
        cout<<"请确认修改后的商品属性："<<endl;
        Data::product_info*p=get_commodity(p_id);
        cout<<"商品ID："<<p->product_ID<<endl;
        cout<<"商品名称："<<p->product_name<<endl;
        cout<<"商品价格：";
        printf("%.1f\n",p->price);
        cout<<"商品描述："<<discription<<endl;
        string instr="UPDATE commodity SET 描述 = "+discription+" WHERE 商品ID = "+p_id;
        get_sql(instr);
    }
    else if(option=="3"){
        cout<<"请输入修改后的商品数量：";
        string quantity;
        cin>>quantity;
        cout<<"请确认修改后的商品属性："<<endl;
        Data::product_info*p=get_commodity(p_id);
        cout<<"商品ID："<<p->product_ID<<endl;
        cout<<"商品名称："<<p->product_name<<endl;
        cout<<"商品价格：";
        printf("%.1f\n",p->price);
        cout<<"商品数量："<<quantity<<endl;
        cout<<"商品描述："<<p->discription<<endl;
        string instr="UPDATE commodity SET 数量 = "+quantity+" WHERE 商品ID = "+p_id;
        get_sql(instr);
    }
    show_data();
}
void Seller::seller_history_order(){
    string instr="SELECT * FROM order WHERE 卖家ID = "+seller_id;
    if(!get_sql(instr)){
        cout<<"没有历史订单"<<endl;
    }
}
void Seller::seller_interface(){
    while(1){
        
        cout<<"**************************************卖家功能*************************************"<<endl;
        cout<<"1.发布商品 2.查看发布商品 3.修改商品信息 4.下架商品 5.查看历史订单 6.返回用户主界面"<<endl;
        cout<<"************************************************************************************"<<endl;
        cout<<"请选择功能："<<endl;
        string option;
        do{
            cin>>option;
            if(option!="1"&&option!="2"&&option!="3"&&option!="4"&&
            option!="5"&&option!="6"){
                cout<<"选项错误，请重新选择：";
            }
        }while(option!="1"&&option!="2"&&option!="3"&&option!="4"&&
            option!="5"&&option!="6");
        
        if(option=="1"){
            //post product
            seller_post_product();
        }
        else if(option=="2"){
            //show products
            seller_show_product_list();
        }
        else if(option=="3"){
            //change products
            seller_change_product();
        }
        else if(option=="4"){
            //remove
            seller_remove_product();
        }
        else if(option=="5"){
            //history order
            seller_history_order();
        }
        else if(option=="6"){
            //back to user interface
            break;
        }
    }
}
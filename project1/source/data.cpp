#include"../includes/data.h"
#include<fstream>
#include<iostream>
#include<sstream>
bool Data::get_sql(string sql_instr){
    istringstream in(sql_instr);
    string instrs[20];
    string tmp;
    int cnt=0;
    //split instr
    while(getline(in,tmp,' ')){
        instrs[cnt]=tmp;
        cnt++;
    }
    int i=1;
    if(instrs[0]=="SELECT"){
        //get colums
        string colums[10];
        int tmpcnt=0;
        while(instrs[i]!="FROM"){
            colums[tmpcnt]=instrs[i];
            i++;
            tmpcnt++;
        }
        //jump from
        i++;
        //get tablename
        string tablename=instrs[i];
        //get conditon if exist
        if(i!=cnt){
            
        }
    }
    else if(instrs[0]=="INSERT"){

    }
    else if(instrs[0]=="UPDATE"){

    }
    else if(instrs[0]=="DELETE"){

    }
}
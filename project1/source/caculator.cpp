#include"../includes/caculator.h"
#include<iostream>
using namespace std;
void Caculator::trans(string exp){
    cout<<exp<<endl;
    int begin,end;
    begin=end=0;
    exp+=";";
    while(end<int(exp.length())){
        //cout<<"begin:"<<exp[begin]<<"  end:"<<exp[end]<<":";
        if(exp[end]!='+'&&exp[end]!='-'&&exp[end]!='('&&exp[end]!=')'&&exp[end]!='*'&&exp[end]!=';'){
            //cout<<"go next"<<endl;
            end++;
        }
        else{
            //前为符号
            if(begin==end){
                //cout<<"get symbol:";
                string tmp=exp.substr(begin,1);
                //cout<<tmp<<endl;
                add_element(tmp);
                //cout<<"/"<<tmp<<" ";
                begin++;
                end++;
            }
            //前不为符号
            else{
                //cout<<"get num and symbol: ";
                string tmp=exp.substr(begin,end-begin);
                string tmp2=exp.substr(end,1);
                //cout<<tmp<<" and "<<tmp2<<endl;
                R.push_back(tmp);
                if(exp[end]=='-'){
                    string add_plus="+";
                    //cout<<"add a extra plus:"<<add_plus<<endl;
                    //cout<<"/&+";
                    add_element(add_plus);
                }
                
                add_element(tmp2);
                //cout<<"/"<<tmp<<" "<<"/"<<tmp2<<" ";
                end++;
                begin=end;
            }
        }
    }
    cout<<endl;
    //清空符号栈
    while(!S.empty()){
        R.push_back(S.top());
        S.pop();
    }
    vector<string>::iterator i;
    for(i=R.begin();i<R.end();i++){
        cout<<*i<<"  ";
    }
    cout<<endl;
    figure();
}
void Caculator::add_element(string e){
    //level ):0  (:1  +:2  *:3  -:4
    if(e=="+"||e=="-"||e=="*"||e=="("||e==")"||e==";"){
        if(S.empty()){
            cout<<"in:"<<e<<endl;
            S.push(e);
        }
        else{
            if(e=="+"){
                cout<<"in : + ";
                while(!S.empty()&&(S.top()=="+"||S.top()=="*"||S.top()=="-")){
                        string tmp=S.top();
                        cout<<"out : "<<tmp<<endl;
                        S.pop();
                        R.push_back(tmp);
                }
                S.push(e);
            }
            else if(e=="-"){
                cout<<"in - : ";
                while(!S.empty()&&S.top()=="-"){
                    string tmp=S.top();
                    cout<<"out  "<<tmp<<endl;
                    S.pop();
                    R.push_back(tmp);
                }
                S.push(e);
            }
            else if(e=="*"){
                cout<<"int: *,";
                while(!S.empty()&&(S.top()=="-"||S.top()=="*")){
                    string tmp=S.top();
                    cout<<"out  "<<tmp<<" ";
                    S.pop();
                    R.push_back(tmp);
                }
                S.push(e);
            }
            else if(e=="("){
                cout<<"in: ("<<endl;
                S.push(e);
            }
            else if(e==")"){
                cout<<"get  ):";
                while(!S.empty()&&S.top()!="("){
                    string tmp=S.top();
                    cout<<"out: "<<tmp<<endl;
                    S.pop();
                    R.push_back(tmp);
                }
                S.pop();
            }
            else if(e==";"){
                cout<<"get :;";
                while(!S.empty()){
                    string tmp=S.top();
                    cout<<" out: "<<tmp<<endl;
                    S.pop();
                    R.push_back(tmp);
                }
            }
            else{
                cout<<"element error."<<endl;
                exit(-1);
            }
            
        }
    }
    else{
        cout<<"in:"<<e<<endl;
        R.push_back(e);
    }
}
float Caculator::figure(){
    vector<string>::iterator i;
    for(i=R.begin();i<R.end();i++){
        if(*i=="+"){
            string op1=F.top();
            F.pop();
            string op2=F.top();
            F.pop();
            cout<<"get top:"<<op1<<"  "<<op2<<endl;
            float tmp=stof(op1.c_str())+stof(op2.c_str());
            string res=to_string(tmp);
            cout<<"push back:"<<res<<endl;
            F.push(res);
        }
        else if(*i=="-"){
            string op=F.top();
            F.pop();
            cout<<"get top: "<<op<<endl;
            float tmp=-stof(op.c_str());
            string res=to_string(tmp);
            cout<<"push back:"<<res<<endl;
            F.push(res);
        }
        else if(*i=="*"){
            string op1=F.top();
            F.pop();
            string op2=F.top();
            F.pop();
            cout<<"get top:"<<op1<<"  "<<op2<<endl;
            float tmp=stof(op1.c_str())*stof(op2.c_str());
            string res=to_string(tmp);
            cout<<"push back:"<<res<<endl;
            F.push(res);
        }
        else{
            string tmp=*i;
            F.push(tmp);
            cout<<"push:"<<tmp<<endl;
        }
    }
    cout<<F.top()<<endl;
    cout<<stof(F.top().c_str())<<endl;
    return 0;
}
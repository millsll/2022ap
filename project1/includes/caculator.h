#ifndef CACULATOR
#define CACULATOR
#include<stack>
#include<vector>
#include<string>
using namespace std;
class Caculator
{
    public:
    float figure_exp(string exp);
    
    private:
    stack<string>S;
    stack<string>F;
    vector<string>R;
    void add_element(string e);
    float figure();
    void trans(string exp);
};
#endif
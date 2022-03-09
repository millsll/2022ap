#ifndef CACULATOR
#define CACULATOR
#include<stack>
#include<string>
using namespace std;
class Caculator
{
    public:
    string trans(string exp);
    private:
    stack<string>S;
};
#endif
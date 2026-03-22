#include <iostream> 

struct RVO {
    int a;
    char b;
    double c;
};

RVO createRVO(int a,char b, double c) {
    return RVO{a,b,c};
}
int main (){
    RVO rvo = createRVO(1,'x',3.14);
    return 0;
}
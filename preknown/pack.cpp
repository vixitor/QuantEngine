#include <iostream> 

#pragma pack(push, 1)
union A { 
    int i;
    char c;
};
struct B { 
    A a;
    char c;
};
#pragma pack(pop)

union AA {
    int i;
    char c; 
};
struct BB { 
    AA a;
    char c;
};
struct BBB { 
    A a;
    char c;
};

#pragma pack(push, 1) 
struct BBBB { 
    AA a;
    char c;
};
#pragma pack(pop) 
int main(){
    std::cout<<sizeof(B) <<" "<< sizeof(BB)<<" "<<sizeof(BBB)<<" "<<sizeof(BBBB)<<"\n";
    return 0;
}
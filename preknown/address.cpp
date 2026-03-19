#include <vector>
#include <cstdint>
#include <iostream>
struct Obj {
    int16_t x;
    int32_t y;
};
int main(){
    std::vector<Obj> obj = {{1,2}, {3,4}, {5,6}, {7,8}};
    std::cout<<sizeof(Obj)<<"\n";
    std::cout<<(&obj[3]) <<" "<<(&obj[0])<<"\n";
    const auto index = &obj[3] - &obj[0];
    std::cout<<index<<"\n";
    return 0;
}
#include "../modules/thread_utils.h"
auto dummyFunction(int a,int b,bool sleep){
    std::cout<<"dummyFunction("<<a<<","<<b<<")"<<std::endl;
    std::cout<<"dummyFunctionOutput "<<a + b<<std::endl;
    if(sleep){
        std::cout<<"dummy sleeping ... "<<std::endl;
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(5s);
    }
    std::cout<<"dummyFunction done"<<std::endl;
}
int main(){
    auto t1 = createAndStartThread(-1, "dummy1", dummyFunction, 1,2,false);
    auto t2 = createAndStartThread(1, "dummy2", dummyFunction, 3, 4, true);
    std::cout<<"main waiting for threads to be done"<<std::endl;
    t1->join();
    t2->join();
    std::cout<<"main exit"<<std::endl;
    return 0;
}
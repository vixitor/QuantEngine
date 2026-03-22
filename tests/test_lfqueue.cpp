#include "../include/lf_queue.h"
#include "../include/thread_utils.h"
#include "../include/macros.h"
#include <cstddef>
#include <iostream> 
#include <thread>
struct MyStruct { 
    int x,y,z;
};
auto ConsumeFunc(LFQueue<MyStruct>* lfq) {
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(5s);
    while(lfq->size()) {
        auto data = lfq->getNextToRead();
        if(data == nullptr){
            FATAL("nullptr error");
        }
        std::cout<<"consumer receive data "<<data->x<<" "<<data->y<<" "<<data->z<<" lfq size() "<<lfq->size()<<std::endl;
        lfq->updataReadIndex();
        std::this_thread::sleep_for(1s);
    }
    std::cout<<"consumeFunction exit"<<std::endl;
}
int main(){
    LFQueue<MyStruct> lfq(20);
    auto t = createAndStartThread(-1, "ConsumeFunction", ConsumeFunc, &lfq);
    for(int i = 0;i < 10;i++){
        auto d = lfq.getNextToWriteTo();
        *d = {i, i + 1, i + 2};
        lfq.updateWriteIndex();
        std::cout<<"main construct elem"<<d->x<<" "<<d->y<<" "<<d->z<<" lfq size "<<lfq.size()<<std::endl;
        using namespace std::literals::chrono_literals;
    }
    std::cout<<"main construct exit"<<std::endl;
    t->join();
    return 0;
}
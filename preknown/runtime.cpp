#include <cstdio>

class Base { 
public: 
    virtual void placeHolder() { 
        printf("Base::placeHolder\n");
    }
};

class Derived : public Base {
public:
    void placeHolder() override {
        printf("Derived::placeHolder\n");
    }
};

int main(){
    Base* basePtr = new Derived();
    basePtr->placeHolder(); // This will call Derived::placeHolder due to virtual dispatch
    delete basePtr;
    return 0;
}

#include <cstdio> 

template <typename Derived>
class Base {
public: 
    void placeHolder() { 
        static_cast<Derived*>(this)->placeHolderImpl();
    }
};

class Derived : public Base<Derived> {
public:
    void placeHolderImpl() {
        printf("Derived::placeHolderImpl\n");
    }
};

int main () {
    Derived d;
    d.placeHolder(); // This will call Derived::placeHolderImpl due to static_cast
    return 0;
}
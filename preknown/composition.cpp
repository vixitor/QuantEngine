#include <cstdio>
#include <vector>

struct Order {
    int id;
    double price;
};

class InheritanceOrderBook : public std::vector<Order> {
};

class CompositionOrderBook {
private:
    std::vector<Order> orders;
public:
    auto size() const { 
        return orders.size(); 
    }
};

int main () {
    InheritanceOrderBook i_book;
    CompositionOrderBook c_book;
    printf("InheritanceOrderBook::size %lu, CompositionOrderBook::size %lu\n", sizeof(InheritanceOrderBook), sizeof(CompositionOrderBook));
    printf("std::vector<Order>::size %lu\n", sizeof(std::vector<Order>));
    printf("Order size %lu\n", sizeof(Order));
    printf("int size %lu, double size %lu\n", sizeof(int), sizeof(double));
    return 0;
}
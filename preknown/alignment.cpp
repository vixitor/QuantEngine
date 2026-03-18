#include <cstdio>
#include <cstddef> 
#include <cstdint>

struct PoorlyAligned {
    char a;
    uint16_t b;
    double c;
    int16_t d;
};

struct WellAligned {
    double c;
    int16_t d;
    uint16_t b;
    char a;
};

#pragma pack(push, 1)
struct Packed {
    char a;
    uint16_t b;
    double c;
    int16_t d;
};
#pragma pack(pop)

int main() { 
    printf("offset a %zu, offset b %zu, offset c %zu, offset d %zu\n size of PoorlyAligned %zu\n", 
        offsetof(PoorlyAligned, a), offsetof(PoorlyAligned, b), offsetof(PoorlyAligned, c), offsetof(PoorlyAligned, d), sizeof(PoorlyAligned));
    printf("offset a %zu, offset b %zu, offset c %zu, offset d %zu\n size of WellAligned %zu\n", 
        offsetof(WellAligned, a), offsetof(WellAligned, b), offsetof(WellAligned, c), offsetof(WellAligned, d), sizeof(WellAligned));
    printf("offset a %zu, offset b %zu, offset c %zu, offset d %zu\n size of Packed %zu\n", 
        offsetof(Packed, a), offsetof(Packed, b), offsetof(Packed, c), offsetof(Packed, d), sizeof(Packed));
     return 0;
}
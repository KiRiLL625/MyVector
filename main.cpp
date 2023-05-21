#include <iostream>
#include "Vector.h"
int main() {
    Vector v1;
    v1.pushBack(1);
    v1.pushBack(2);
    v1.pushBack(3);
    v1.pushFront(123);
    Vector::Iterator it = v1.begin();
    std::cout << *it << std::endl;
    ++it;
    std::cout << *it << std::endl;
    ++it;
    v1.end();
    return 0;
}

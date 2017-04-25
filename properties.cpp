#include <iostream>

#include "include/properties.h"

struct p1 : public property<p1, bool, false> {};
struct p2 : public property<p2, int, 42> {};

class test :
    public object<test>,
    public with_property<p1>
{};

int main() {
    test x;
    std::cout << x.get<p1>() << std::endl;
    x.set<p1>(true); std::cout << x.get<p1>() << std::endl;
    x.set<p1>(false); std::cout << x.get<p1>() << std::endl;
    x.set<p1>(true); std::cout << x.get<p1>() << std::endl;
    std::cout << x.get<p2>() << std::endl;
    std::cout << x.has<p1>() << std::endl;
    std::cout << x.has<p2>() << std::endl;
}

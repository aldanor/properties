#include <iostream>

#include "include/properties.h"

using namespace properties;

struct p1 : public property<p1, bool, false> {};
struct p2 : public property<p2, int, 42> {};

class test
    : public object<test>,
      public with_property<p1>
{};

int main() {
    test x;
    std::cout << std::boolalpha;
    std::cout << "get(p1): " << x.get<p1>() << std::endl;
    x.set<p1>(true); std::cout << "get(p1): " << x.get<p1>() << std::endl;
    x.set<p1>(false); std::cout << "get(p1): " << x.get<p1>() << std::endl;
    x.set<p1>(true); std::cout << "get(p1): " << x.get<p1>() << std::endl;
    std::cout << "get(p2): " << x.get<p2>() << std::endl;
    std::cout << "get(p1): " << x.has<p1>() << std::endl;
    std::cout << "has(p2): " << x.has<p2>() << std::endl;
    std::cout << "get_or(p1, false): " << x.get_or<p1>(false) << std::endl;
    std::cout << "get_or(p2, 43): " << x.get_or<p2>(43) << std::endl;
}

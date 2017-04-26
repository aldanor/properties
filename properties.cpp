#include <iostream>

#include "include/properties.h"

using namespace properties;

struct p1 : public property<p1, bool, false> {
    using property<p1, bool, false>::property;
};

struct p2 : public property<p2, int, 42> {
    using property<p2, int, 42>::property;
};

class test
    : public object<test>,
      public with_property<p1>
{};

class baz
    : public test,
      public object<baz>,
      public with_property<p2>
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

    baz b;
    std::cout << p1::has(b) << std::endl;
    std::cout << p2::has(b) << std::endl;
    std::cout << p1::get(b) << std::endl;
    std::cout << p2::get(b) << std::endl;
    p1::set(b, true);
    p2::set(b, true);
    std::cout << p1::get(b) << std::endl;
    std::cout << p2::get(b) << std::endl;
    std::cout << p1::get_or(b, false) << std::endl;
    std::cout << p2::get_or(b, 44) << std::endl;

    set(b);
    set(b, p1{false});
    set(b, p1{true}, p2{142});
    std::cout << p1::get(b) << std::endl;
    std::cout << p2::get(b) << std::endl;
}

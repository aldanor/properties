#include <iostream>
#include <type_traits>

struct property_base {};

template<typename P, typename V, V value>
class property : public property_base {
public:
    constexpr property() = default;

    template<typename T>
    static constexpr V get_property(const T& obj) noexcept {
        return static_cast<const P&>(obj).value_;
    }

    template<typename T>
    static void set_property(T& obj, V v) noexcept {
        static_cast<P&>(obj).value_ = v;
    }

    V value_ = value;
};

template<typename P>
constexpr bool is_property = std::is_base_of<property_base, P>{};

template<typename P>
struct tag {
    using type = P;
};

template<typename T>
struct writer_base {
    template<typename P, typename V, typename = std::enable_if_t<is_property<P>>>
    T& set(const V& value) noexcept {
        P::set_property(static_cast<T&>(*this), value);
        return static_cast<T&>(*this);
    }

    template<typename P, std::enable_if_t<std::is_base_of<P, T>{} && is_property<P>>* = nullptr>
    auto get() const noexcept {
        return P::get_property(static_cast<const T&>(*this));
    }

    template<typename P, std::enable_if_t<!std::is_base_of<P, T>{} && is_property<P>>* = nullptr>
    constexpr auto get() const noexcept {
        return P::get_property(P{});
    }

    template<typename P>
    constexpr bool has() const noexcept {
        return std::is_base_of<P, T>{};
    }
};

struct p1 : public property<p1, bool, false> {};
struct p2 : public property<p2, int, 42> {};

class writer :
    public writer_base<writer>,
    public p1
{};

int main() {
    writer foo;
    std::cout << foo.get<p1>() << std::endl;
    foo.set<p1>(true); std::cout << foo.get<p1>() << std::endl;
    foo.set<p1>(false); std::cout << foo.get<p1>() << std::endl;
    foo.set<p1>(true); std::cout << foo.get<p1>() << std::endl;
    std::cout << foo.get<p2>() << std::endl;
    std::cout << foo.has<p1>() << std::endl;
    std::cout << foo.has<p2>() << std::endl;
}

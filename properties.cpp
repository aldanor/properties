#include <iostream>
#include <type_traits>

struct property_base {};

template<typename P>
constexpr bool is_property = std::is_base_of<property_base, P>{};

template<typename>
class with_property;

template<typename T, typename P>
constexpr bool has_property = std::is_base_of<with_property<P>, T>{};

template<typename P, typename V, V value>
struct property : public property_base {
    using value_type = V;

    constexpr property() = default;

    template<typename T>
    static constexpr V get_property(const T& obj) noexcept {
        return static_cast<const with_property<P>&>(obj)();
    }

    template<typename T>
    static void set_property(T& obj, V v) noexcept {
        static_cast<with_property<P>&>(obj)() = v;
    }

private:
    V value_ = value;

    friend with_property<P>;
};

template<typename P>
class with_property {
private:
    template<typename, typename V, V value>
    friend struct property;

    typename P::value_type& operator()() {
        return property_.value_;
    }

    constexpr const typename P::value_type& operator()() const {
        return property_.value_;
    }

    P property_ = {};
};

template<typename T>
struct object {
    template<typename P, typename V, typename = std::enable_if_t<is_property<P>>>
    T& set(const V& value) noexcept {
        P::set_property(static_cast<T&>(*this), value);
        return static_cast<T&>(*this);
    }

    template<typename P, std::enable_if_t<is_property<P> && has_property<T, P>>* = nullptr>
    auto get() const noexcept {
        return P::get_property(static_cast<const T&>(*this));
    }

    template<typename P, std::enable_if_t<is_property<P> && !has_property<T, P>>* = nullptr>
    constexpr auto get() const noexcept {
        return P::get_property(with_property<P>{});
    }

    template<typename P>
    constexpr bool has() const noexcept {
        return has_property<T, P>;
    }
};

// -----------------------------------------------------------------------------------------------

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

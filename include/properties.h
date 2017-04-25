#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <type_traits>

namespace properties {

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

    template<typename P, std::enable_if_t<is_property<P> && has_property<T, P>>* = nullptr>
    auto get_or(typename P::value_type value) const noexcept {
        return get<P>();
    }

    template<typename P, std::enable_if_t<is_property<P> && !has_property<T, P>>* = nullptr>
    constexpr auto get_or(typename P::value_type value) const noexcept {
        return value;
    }

    template<typename P, typename = std::enable_if_t<is_property<P>>>
    constexpr bool has() const noexcept {
        return has_property<T, P>;
    }
};

}  // namespace properties

#endif  // PROPERTIES_H_

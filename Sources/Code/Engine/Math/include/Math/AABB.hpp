#ifndef __AABB_HPP__
#define __AABB_HPP__

#include "Math/Vector.hpp"

namespace Math {

template<typename T>
struct AABB {
public:

    AABB() = default;
    AABB(const T& bt, const T& tp) :
        bot(bt), top(tp) {}
    AABB(const AABB& other) :
        bot(other.bot), top(other.top) {}
    template<typename F>
    explicit AABB(const F& val) :
        bot(val), top(val) {}

    AABB& operator=(const AABB& other) {
        bot = other.bot;
        top = other.top;
        return *this;
    }

    T getSize() const {
        return top - bot;
    }
    T getCenter() const {
        return (top + bot) / static_cast<T>(2);
    }
    void setCenter(const T& pt) {
        T shift = pt - getCenter();
        bot = bot + shift;
        top = top + shift;
    }

public:

    T bot;
    T top;
};

template<typename T>
AABB<T> operator+(const AABB<T>& aabb, const T& pos) {
    return AABB<T>(aabb.bot + pos, aabb.top + pos);
}

template<typename T>
bool operator==(const AABB<T>& first, const AABB<T>& second) {
    return first.top == second.top && first.bot == second.bot;
}

template<typename T>
bool operator!=(const AABB<T>& first, const AABB<T>& second) {
    return !(first == second);
}

} // namespace Math

typedef Math::AABB<Vec2> AABB2D;
typedef Math::AABB<Vec2i> AABB2Di;
typedef Math::AABB<Vec3> AABB;

#endif /* __AABB_HPP__ */
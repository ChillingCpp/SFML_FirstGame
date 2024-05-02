#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
// circle bounding box class, useful for circle sprite, body

namespace sf
{
    template <typename T>
    class Circle
    {

    public:
        Circle() = default;
        Circle(T x, T y, float radius) : position(x, y), radius(radius)
        {
        }
        Circle(const Vector2<T> &position, float radius) : position(position), radius(radius)
        {
        }
        template <typename U>
        Circle(const Circle<U> &circle) : position(Vector2<T>(circle.position)), radius(static_cast<T>(circle.radius))
        {
        }

        bool contains(T x, T y)
        {
            T xx = x - position.x;
            T yy = y - position.y;
            float dist = std::hypot(xx, yy);
            return dist <= radius;
        }
        bool contains(const Vector2<T> &point)
        {
            return contains(point.x, point.y);
        }

    public:
        float radius;
        Vector2<T> position;
    };
    typedef Circle<int> IntCircle;
    typedef Circle<float> FloatCircle;
} // namespace sf

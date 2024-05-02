#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
namespace sf
{
    template <typename T>
    class Capsule
    {
    public:
        Capsule() = default;
        // construct a capsule to sweep
        Capsule(const Vector2<T> &begin, const Vector2<T> &end, float r):
        begin(begin),
        end(end),
        radius(r)
        {

        }
        // construct a capsule with given size (been scaled) to make a bounding box
        Capsule(const Vector2<T> &size, const sf::Vector2f &scale, const Vector2<T> &position, float rotation)
        {
            size.x *= scale.x;
            size.y *= scale.y;
            rotation = std::fmod(rotation, 360.f);
            rotation -= 180.f;
            if (size.x < size.y){
                radius = size.x;
                Vector2<T> start(position.x, position.y - size.y + radius);
                Vector2<T> final(position.x, position.y + size.y - radius);
                Vector2<T> center = (start + final) /2.f;
                begin = position + getRotated(start - center);
                end = position + getRotated(final - center);
            }
            else{
                radius = size.y;
                Vector2<T> start(position.x - size.x + radius, position.y);
                Vector2<T> final(position.x + size.x - radius, position.y);
                Vector2<T> center = (start + final) /2.f;
                begin = position + getRotated(start - center);
                end = position + getRotated(final - center);
            }

        }
        template <typename U>
        Capsule(Capsule<U> &capsule): begin(Vector2<T>(capsule.begin)), end(Vector2<T>(capsule.end)), radius(static_cast<T>(capsule.radius))
        {

        }
        inline const Vector2<T> getRotated(const Vector2<T> &vec, float angle){
            const double angleRad = -angle * 3.141592654f / 180.f;
            const double newX = vec.x * cos(angleRad) - vec.y * sin(angleRad);
            const double newY = vec.x * sin(angleRad) + vec.y * cos(angleRad);

            return Vector2<T>(newX, newY);
        }
        bool contains(T x, T y){
            //
        }
    public:
        float radius;
        Vector2<T> begin;
        Vector2<T> end;
        bool sweep = false; // does using capsule to sweep or use as a bounding box ? 
    };
typedef Capsule<int> IntCapsule;
typedef Capsule<float> FloatCapsule;
} // namespace sf

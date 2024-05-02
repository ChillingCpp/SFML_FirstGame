#pragma once
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
/// @brief an OBB is a aabb but have m_rotation, in range 0 to 360 degree
namespace sf
{
    template <typename T>
    class Obb
    {
    public:

        
        Obb() : position(0, 0), size(0, 0), m_rotation(0)
        {
            
        };
        Obb(T sizex, T sizey, T posx, T posy, float angle):
        position(Vector2<T>(posx, posy)),
        size(Vector2<T>(sizex, sizey)),
        m_rotation(std::fmod(angle, 360.f))
        {
            m_rotation < 0 ? m_rotation += 360.f : m_rotation;
            m_rotation -=180;
        };
        
        Obb(const Vector2<T> &size, const Vector2<T> &position, float angle):
        position(position),
        size(size),
        m_rotation(std::fmod(angle, 360.f))
        {
            m_rotation < 0 ? m_rotation += 360.f : m_rotation;
            m_rotation -=180;
        };
        template <typename U>
        explicit Obb(const Obb<U> &obb): 
        position(Vector2<T>(obb.position)),
        size(Vector2<T>(obb.position)), 
        m_rotation(static_cast<T>(obb.m_rotation))
        {

        }
        const Vector2f getRotated(const Vector2f &vec, float angle){
            const double angleRad = -angle * 3.141592654f / 180.f;
            const double newX = vec.x * cos(angleRad) - vec.y * sin(angleRad);
            const double newY = vec.x * sin(angleRad) + vec.y * cos(angleRad);

            return Vector2f(newX, newY);
        }
        bool contains(const Vector2<T> &point)
        {
            float minX = std::min(position.x - size.x / 2, position.x + size.x / 2);
            float maxX = std::max(position.x - size.x / 2, position.x + size.x / 2);
            float minY = std::min(position.y - size.y / 2, position.y + size.y / 2);
            float maxY = std::max(position.y - size.y / 2, position.y + size.y / 2);

            Vector2f newp = getRotated(point - position, m_rotation);
            return (newp.x >= minX) && (newp.x <= maxX) && (newp.y >= minY) && (newp.y <= maxY);
        }
        bool contains(T x, T y)
        {
            float minX = std::min(position.x - size.x / 2, position.x + size.x / 2);
            float maxX = std::max(position.x - size.x / 2, position.x + size.x / 2);
            float minY = std::min(position.y - size.y / 2, position.y + size.y / 2);
            float maxY = std::max(position.y - size.y / 2, position.y + size.y / 2);

            Vector2f point = {x, y};

            Vector2f newp = getRotated(point - position, m_rotation);
            return (newp.x >= minX) && (newp.x <= maxX) && (newp.y >= minY) && (newp.y <= maxY);
        }
        // bool intersect(const LineSegment &line){

        // }
        // std::pair<Vector2f> intersect(const LineSegment &line){
            

        //     // return 2 intersect point, or only 1, and another is at inf, inf
        // }
        // 0 -> number of points - 1 
        Vector2<T> getPoint(int index){
            
            float minX = std::min(position.x - size.x / 2, position.x + size.x / 2);
            float maxX = std::max(position.x - size.x / 2, position.x + size.x / 2);
            float minY = std::min(position.y - size.y / 2, position.y + size.y / 2);
            float maxY = std::max(position.y - size.y / 2, position.y + size.y / 2);
            Vector2f point = {minX, minY};
            switch (index)
            {
            case 1:
                point = {maxX, minY};
                break;
            case 2:
                point = {maxX, maxY};
                break;
            case 3:
                point = {minX, maxY};
                break;
            
            default:
                break;
            }
            Vector2f rotatedpoint = (point-position, m_rotation);
            return rotatedpoint;

        }
        const Obb intersect(const Obb &obb1){
            
            Obb intersection = Obb<T>(0, 0, 0, 0, 0);
            if (obb1.m_rotation != m_rotation) return intersection; // it need return a set of point represent convexhull
            return intersect(obb1, intersection);
        }
        const Obb intersect(const Obb &obb1, const Obb& intersection)
        {
            
            T obb1_left = obb1.position.x-obb1.size.x/2;
            T obb1_top = obb1.position.y-obb1.size.y/2;

            T left = position.x - size.x/2;
            T top = position.y -size.y/2;

            if (obb1.m_rotation != m_rotation) return intersection;
            T r1MinX = std::min(left, static_cast<T>(left + size.x));
            T r1MaxX = std::max(left, static_cast<T>(left + size.x));
            T r1MinY = std::min(top, static_cast<T>(top + size.y));
            T r1MaxY = std::max(top, static_cast<T>(top + size.y));

            // Compute the min and max of the second rectangle on both axes
            T r2MinX = std::min(obb1_left, static_cast<T>(obb1_left + obb1.size.x));
            T r2MaxX = std::max(obb1_left, static_cast<T>(obb1_left + obb1.size.x));
            T r2MinY = std::min(obb1_top, static_cast<T>(obb1_top + obb1.size.y));
            T r2MaxY = std::max(obb1_top, static_cast<T>(obb1_top + obb1.size.y));

            // Compute the intersection boundaries
            T interLeft   = std::max(r1MinX, r2MinX);
            T interTop    = std::max(r1MinY, r2MinY);
            T interRight  = std::min(r1MaxX, r2MaxX);
            T interBottom = std::min(r1MaxY, r2MaxY);

            // If the intersection is valid (positive non zero area), then there is an intersection
            if ((interLeft < interRight) && (interTop < interBottom))
            {
                intersection = Obb<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop, m_rotation);
            }
            return intersection;
        }
        // const Obb& combine(const Obb& Obb1){

        // }
       

    public:
        Vector2<T> size, position;
        float m_rotation; // degree clockwise

    private:
    };
typedef Obb<int>   IntObb;
typedef Obb<float> FloatObb;
};
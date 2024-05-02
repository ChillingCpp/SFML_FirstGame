/// this file wrap all intersection algorithm for object: convexhull, obb, aabb, circle, capsule, line segment

#include <SFML/Graphics/BoundingBox.hpp>

struct RayCastInput{

};
struct RayCastOutput{
    
};
namespace sf
{
    template <typename T>
    struct Contact{
        Vector2<T> point;
        Vector2<T> normal;
        T penetration_depth;
        int id;
    };
    class Collisions
    {
        template <typename T>
        T dot(const Vector2<T> &v1, const Vector2<T> &v2){
            return v1.x*v2.x + v1.y*v2.y;
        }
        template <typename T>
        T cross(const Vector2<T> &v1, const Vector2<T> &v2){
            return v1.x*v2.y - v1.y*v2.x;
        }
        template <typename T>
        T distance(const Vector2<T> &p1, const Vector2<T> &p2){
            return std::hypot(p2.x - p1.x, p2.y - p1.y);
        }
        template <typename T>
        T length(const Vector2<T> &vec){
            return std::hypot(vec.x, vec.y);
        }
        template <typename T>
        Vector2<T> normalize(const sf::Vector2<T> &vec){
            return vec / length(vec);
        }
        template <typename T>
        Vector2<T> closetPointOnLineSegment(const Vector2<T> &point, const LineSegment<T> &line){
            
            float k = dot(point - line.start, line.vector()) / line.lengthsquared();
            if (k <= 0.f) return line.start;
            if (k >= 1.f) return line.end;
            return line.start + k*line.vector();
        }
        // find contact point and normal;
        template <typename T>
        Contact<T> CircleLineSegment(const Circle<T> &circle, const LineSegment<T>& line){
            Vector<T> closetPoint = closetPointOnLineSegment(circle.position, line);
            float dist = distance(closetPoint, circle.position);
            Contact<T> contactinfo;
            if (dist > radius) return contactinfo;
            contactinfo.point = closetPoint;
            contactinfo.normal = normalize(circle.position - closetPoint);
            contactinfo.penetration_depth = circle.radius - dist;
            return contactinfo;
        } 
    };
};
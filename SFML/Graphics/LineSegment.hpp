#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <numeric>
#include <cmath>
namespace sf
{
    template <typename T>
    class LineSegment
    {
    public:
        LineSegment() : m_start({0, 0}),
                        m_end({0, 0}){

                        };
        /*
        // construct a line with both point
        */
        LineSegment(Vector2<T> _s, Vector2<T> _e) : m_start(_s), m_end(_e)
        {
        }
        ~LineSegment(){};
        T slope()
        {
            Vector2<T> d = m_end - m_start;
            return (d.x != 0) ? d.y / d.x : std::numeric_limits<T>::infinity();
        }
        T angle()
        {
            Vector2<T> d = m_end - m_start;
            return (d.x != 0) ? std::atan(d.y / d.x) : 90.f;
        }
        T length()
        {
            Vector2<T> vec = m_end - m_start;
            return std::hypot(vec.x, vec.y);
        }
        T lengthsquared()
        {
            return length() * length();
        }
        Vector2<T> absolutesize()
        {
            return {std::abs(m_end.x - m_start.x), std::abs(m_end.y - m_start.y)};
        }
        Vector2<T> direction()
        {
            Vector2<T> vec = m_end - m_start;
            vec.x /= length();
            vec.y /= length();
            return vec;
        }
        Vector2<T> vector()
        {
            return m_end - m_start;
        }
        Vector2<T> invertvec()
        {
            return m_start - m_end;
        }
        Vector2<T> invertdirection()
        {
            return -direction();
        }
        T minX()
        {
            return m_start.x < m_end.x ? m_start.x : m_end.x;
        }

        T minY()
        {
            return m_start.y < m_end.y ? m_start.y : m_end.y;
        }

        T maxX()
        {
            return m_start.x > m_end.x ? m_start.x : m_end.x;
        }

        T maxY()
        {
            return m_start.y > m_end.y ? m_start.y : m_end.y;
        }
        T YIntercept(const Vector2<T> &anyPoint, T slope)
        {
            return slope != std::numeric_limits<T>::infinity() ? anyPoint.y - slope * anyPoint.x : slope;
        }
        void setposall(const Vector2<T> &postart, const Vector2<T> &posend)
        {
            m_start = postart;
            m_end = posend;
        }
        void movea(const Vector2<T> &amt1, const Vector2<T> &amt2)
        {
            setposall(m_start + amt1, m_end + amt2);
        }
        // add scaling factor

        
        const Rect<T> &getGlobalBounds()
        {
            m_bound.top = minY();
            m_bound.left = minX();
            m_bound.width = std::abs(m_end.x - m_start.x);
            m_bound.height = std::abs(m_end.y - m_start.y);
            return m_bound;
        }
        const Vector2<T> &intersects(LineSegment<T> &line)
        {
            // m_start = x1, y1
            // m_end = x2, y2
            // line.m_start = x3, y3;
            // line.m_end = x4, y4
            Vector2<T> intersect_point = {std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity()};
            if (line.length() == 0)
                return intersect_point;
            if (length() == 0)
                return intersect_point;

            T denominator = (m_start.x - m_end.x) * (line.m_start.y - line.m_end.y) - (m_start.y - m_end.y) * (line.m_start.x - line.m_end.x);
            if (denominator == 0)
                return intersect_point;
            T t_numerator = (m_start.x - line.m_start.x) * (line.m_start.y - line.m_end.y) - (m_start.y - line.m_start.y) * (line.m_start.x - line.m_end.x);
            T u_numerator = -(m_start.x - m_end.x) * (m_start.y - line.m_start.y) - (m_start.y - m_end.y) * (m_start.x - line.m_start.x);
            T t = t_numerator / denominator;
            T u = u_numerator / denominator;
            if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
            {
                intersect_point = {
                    m_start.x + t * (m_end.x - m_start.x),
                    m_start.y + t * (m_end.y - m_start.y)};
            }

            return intersect_point;
        }

    public:
        Vector2<T> m_start;
        Vector2<T> m_end;

    private:
        Rect<T> m_bound;
    };
typedef LineSegment<int> IntLineSegment;
typedef LineSegment<float> TLineSegment;
};
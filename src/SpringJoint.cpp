#include "SpringJoint.hpp"
#include <numeric>
#include <iostream>
float LineSegment::slope()
{
    sf::Vector2f d = end - start;
    return (d.x != 0) ? d.y / d.x : std::numeric_limits<float>::infinity();
}
float LineSegment::angle()
{
    sf::Vector2f d = end - start;
    return (d.x != 0) ? std::atan(d.y / d.x) : 90.f;
}
float LineSegment::length()
{
    return sk.getLength(end - start);
}
float LineSegment::lengthsquared()
{
    return sk.getLengthsqr(end - start);
}
sf::Vector2f LineSegment::absolutesize()
{
    return {std::abs(end.x - start.x), std::abs(end.y - start.y)};
}
sf::Vector2f LineSegment::direction()
{
    return sk.getNormalized(end - start);
}
sf::Vector2f LineSegment::vector()
{
    return end - start;
}
sf::Vector2f LineSegment::invertvec()
{
    return start - end;
}
sf::Vector2f LineSegment::invertdirection()
{
    return sk.getNormalized(start - end);
}
float LineSegment::minX()
{
    return start.x < end.x ? start.x : end.x;
}

float LineSegment::minY()
{
    return start.y < end.y ? start.y : end.y;
}

float LineSegment::maxX()
{
    return start.x > end.x ? start.x : end.x;
}

float LineSegment::maxY()
{
    return start.y > end.y ? start.y : end.y;
}
float LineSegment::YIntercept(const sf::Vector2f &anyPoint, float slope)
{
    return slope != std::numeric_limits<float>::infinity() ? anyPoint.y - slope * anyPoint.x : slope;
}
void LineSegment::setpostart(const sf::Vector2f &pos)
{
    start = pos;
}
void LineSegment::setposend(const sf::Vector2f &pos)
{
    end = pos;
}
void LineSegment::setposall(const sf::Vector2f &postart, const sf::Vector2f &posend)
{
    setpostart(postart);
    setposend(posend);
}
void LineSegment::movea(const sf::Vector2f &amt1, const sf::Vector2f &amt2)
{
    moves(amt1);
    movee(amt2);
}
void LineSegment::moves(const sf::Vector2f &amt)
{
    start += amt;
}
void LineSegment::movee(const sf::Vector2f &amt)
{
    end += amt;
}
void LineSegment::movea(float x1, float y1, float x2, float y2)
{
    movea({x1, y1}, {x2, y2});
}
void LineSegment::moves(float x, float y)
{
    moves({x, y});
}
void LineSegment::movee(float x, float y)
{
    movee({x, y});
}
void LineSegment::setColorstart(sf::Color &color)
{
    m_colors[0] = color;
}
void LineSegment::setColorend(sf::Color &color)
{
    m_colors[1] = color;
}
void LineSegment::setColorall(sf::Color &color1, sf::Color &color2)
{
    setColorstart(color1);
    setColorend(color2);
}
void LineSegment::setTexcoordstart(sf::Vector2f &texcoord)
{
    m_texcoords[0] = texcoord;
}
void LineSegment::setTexcoordend(sf::Vector2f &texcoord)
{
    m_texcoords[1] = texcoord;
}
void LineSegment::setTexcoordall(sf::Vector2f &texcoord1, sf::Vector2f &texcoord2)
{
    setTexcoordstart(texcoord1);
    setTexcoordend(texcoord2);
}
void LineSegment::draw(sf::RenderTarget &target, sf::RenderStates &states)
{
    m_vertices[0] = sf::Vertex(start, m_colors[0], m_texcoords[0]);
    m_vertices[1] = sf::Vertex(end, m_colors[1], m_texcoords[1]);
    target.draw(&m_vertices[0], m_vertices.size(), sf::Lines);
}
const sf::Vector2f& LineSegment::intersects(LineSegment &line)
{
    // start = x1, y1
    // end = x2, y2
    // line.start = x3, y3;
    // line.end = x4, y4
    intersect_point = {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
    if (line.length() == 0) return intersect_point;
    if (length() == 0) return intersect_point;
    
    float denominator = (start.x - end.x)*(line.start.y - line.end.y) - (start.y -end.y)*(line.start.x - line.end.x);
    if (denominator == 0) return intersect_point;
    float t_numerator = (start.x -line.start.x)*(line.start.y - line.end.y) -(start.y -  line.start.y)*(line.start.x - line.end.x);
    float u_numerator = -(start.x - end.x)*(start.y - line.start.y)-  (start.y - end.y)*(start.x - line.start.x);
    float t = t_numerator / denominator;
    float u = u_numerator/ denominator;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1){
        intersect_point = {
            start.x + t*(end.x - start.x),
            start.y + t*(end.y - start.y)       
        };
    } 
    
    return intersect_point;
}
void SpringJoint::setConstantA(float k, float b)
{
    this->bA = b;
    this->kA = k;
}
void SpringJoint::setConstantB(float k, float b)
{
    this->bB = b;
    this->kB = k;
}
// A is wheels, B is body
void SpringJoint::setAnchorA(const sf::Vector2f &anchorA)
{
    m_anchorA = anchorA;
}
// A is wheels, B is body
void SpringJoint::setAnchorB(const sf::Vector2f &anchorB)
{
    m_anchorB = anchorB;
}
// A is wheels, B is body
void SpringJoint::setBody(RigidBody *bodyA, RigidBody *bodyB)
{
    m_BodyA = bodyA;
    m_BodyB = bodyB;
}
void SpringJoint::setGravity(const sf::Vector2f &gravity)
{
    m_gravity = gravity;
}
// A is wheels, B is body
void SpringJoint::moveAnchorA(const sf::Vector2f &offset)
{
    setAnchorA(m_anchorA + offset);
}
// A is wheels, B is body
void SpringJoint::moveAnchorB(const sf::Vector2f &offset)
{
    setAnchorB(m_anchorB + offset);
}
#pragma once
#include <Body.hpp>

template <typename T>
void printv(const sf::Vector2<T> &vec, char end = '\n')
{
    using namespace std;
    cout << vec.x << ' ' << vec.y << end;
}
template <typename T>
void printr(const sf::Rect<T> &rect, char end = '\n')
{
    using namespace std;
    cout << rect.top << ' ' << rect.left << ' ' << rect.width << ' ' << rect.height << end;
}
class LineSegment
{
public:
    sf::Vector2f start;
    sf::Vector2f end;

    sm::sk sk;

public:
    LineSegment() : start({0, 0}),
                    end({0, 0})
    {
        m_vertices = std::vector<sf::Vertex>(2);
        m_colors = std::vector<sf::Color>(2);
        m_texcoords = std::vector<sf::Vector2f>(2);
    };
    /*
    // construct a line with both point
    */
    LineSegment(sf::Vector2f _s, sf::Vector2f _e) : start(_s), end(_e)
    {
        m_vertices = std::vector<sf::Vertex>(2);
        m_colors = std::vector<sf::Color>(2);
        m_texcoords = std::vector<sf::Vector2f>(2);
    }
    ~LineSegment(){};
    float slope();
    float angle();
    float length();
    float lengthsquared();
    sf::Vector2f absolutesize();
    sf::Vector2f direction();
    sf::Vector2f vector();
    sf::Vector2f invertvec();
    sf::Vector2f invertdirection();
    float minX();
    float minY();

    float maxX();
    float maxY();
    float YIntercept(const sf::Vector2f &anyPoint, float slope);

    const sf::FloatRect& getGlobalBounds(){
        m_bound.top = std::min(start.y, end.y);
        m_bound.left = std::min(start.x, end.x);
        m_bound.width = std::abs(end.x - start.x);
        m_bound.height = std::abs(end.y - start.y);
        return m_bound;
    }
    void setpostart(const sf::Vector2f &pos);
    void setposend(const sf::Vector2f &pos);
    void setposall(const sf::Vector2f &postart, const sf::Vector2f &posend);
    void movea(const sf::Vector2f &amt1, const sf::Vector2f &amt2);
    void moves(const sf::Vector2f &amt);
    void movee(const sf::Vector2f &amt);
    void movea(float x1, float y1, float x2, float y2);
    void moves(float x, float y);
    void movee(float x, float y);
    void setColorstart(sf::Color &color);
    void setColorend(sf::Color &color);
    void setColorall(sf::Color &color1, sf::Color &color2);
    void setTexcoordstart(sf::Vector2f &texcoord);
    void setTexcoordend(sf::Vector2f &texcoord);
    void setTexcoordall(sf::Vector2f &texcoord1, sf::Vector2f &texcoord2);
    void draw(sf::RenderTarget &target, sf::RenderStates &states);

    const sf::Vector2f& intersects(LineSegment &line);

private:
    std::vector<sf::Vertex> m_vertices;
    std::vector<sf::Color> m_colors;
    std::vector<sf::Vector2f> m_texcoords;
    sf::FloatRect m_bound;

    sf::Vector2f intersect_point;
};

class SpringJoint : public LineSegment
{

public:
    SpringJoint() : m_BodyA(nullptr), m_BodyB(nullptr), LineSegment(){};
    SpringJoint(const sf::Vector2f &anchorA, const sf::Vector2f &anchorB) : m_BodyA(nullptr), m_BodyB(nullptr), LineSegment(anchorA, anchorB), m_anchorA(anchorA), m_anchorB(anchorB){};

    SpringJoint(RigidBody *bodyA, RigidBody *bodyB) : LineSegment(), m_BodyA(bodyA), m_BodyB(bodyB){};
    SpringJoint(RigidBody *bodyA, RigidBody *bodyB, const sf::Vector2f &anchorA, const sf::Vector2f &anchorB) : LineSegment(anchorA, anchorB), m_BodyA(bodyA), m_BodyB(bodyB), m_anchorA(anchorA), m_anchorB(anchorB){};
    ~SpringJoint(){m_BodyA = nullptr; m_BodyB = nullptr;}
    /*
    /// setconstant for damping and hooke spring constant
    */
    void setConstantA(float k, float b);

    void setConstantB(float k, float b);

    // A is wheels, B is body
    void setAnchorA(const sf::Vector2f &anchorA);

    // A is wheels, B is body
    void setAnchorB(const sf::Vector2f &anchorB);

    // A is wheels, B is body
    void setBody(RigidBody *bodyA, RigidBody *bodyB);
    
    void setGravity(const sf::Vector2f &gravity);
    // A is wheels, B is body
    void moveAnchorA(const sf::Vector2f &offset);

    // A is wheels, B is body
    void moveAnchorB(const sf::Vector2f &offset);

    void solveposition(float dt)
    {

        sf::Vector2f x1 = start - m_anchorA;
        sf::Vector2f x2 = end - m_anchorB;

        springA = kA * x1;
        springB = kB * x2;

        sf::Vector2f veloA = sk.getLength(m_BodyA->m_velocity) != 0 ? m_BodyA->m_velocity - m_BodyB->m_velocity : sf::Vector2f(0, 0);
        sf::Vector2f veloB = sk.getLength(m_BodyB->m_velocity) != 0 ? m_BodyB->m_velocity - m_BodyA->m_velocity : sf::Vector2f(0, 0);

        // calculate damping force
        dampingA = bA * veloA; // damping = b * (Va -Vb) (relative velocity)
        dampingB = bB * veloB; // damping = b * (Va -Vb) (relative velocity) 
        sf::Vector2f amtA = sk.round(((-(springA + dampingA ) / m_BodyA->m_mass) ));
        amtA == sf::Vector2f(0, 0) ? amtA : amtA += m_gravity;
        sf::Vector2f amtB = sk.round(((-(springB + dampingB )) / m_BodyB->m_mass));
        amtB == sf::Vector2f(0, 0) ? amtB : amtB += m_gravity;
        m_BodyA->m_velocity += sk.round(amtA *dt);
        m_BodyB->m_velocity += sk.round(amtB *dt);

    }

public:
    // this is the origin of spring, if the start spring is move and the anchor isnt
    // A is wheels, B is body
    sf::Vector2f m_anchorA, m_anchorB; 
    sf::Vector2f m_localanchorA, m_localanchorB; // A is wheels, B is body
    sf::Vector2f m_localstart, m_localend; // A is wheels, B is body
    sf::Vector2f m_gravity;
    // sf::Vector2f velocityA = {0, 0}, velocityB = {0, 0};
    // then a force will excert to resolve position, calculate spring force;
    // need local position of origin, start and anchorB pos based on carbody. thinking of using move to move not use setposition
    // anchorA = start, anchorB = end;
    sf::Vector2f springA, springB, dampingA, dampingB, FgA, FgB;
    sf::Vector2f localA, localB;
    RigidBody *m_BodyA; // this is wheels, 
    RigidBody *m_BodyB; // this is body.
    float bA, kA, bB, kB; // b is damping constant, k is spring constant
};
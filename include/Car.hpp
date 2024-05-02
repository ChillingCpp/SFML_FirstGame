#pragma once

#include <iostream>
#include <cstdarg>
#include <SpringJoint.hpp>
#include <Map.hpp>

// another efficient way to draw a car:
// simply add a vector contain wheel position, so we only need a RigidBody wheel to draw instead;
class Car
{

private:
    int WD_types; // 0 is RWD, 1 is FWD, 2 is AWD
    int wheeldrives;

public:
    Car() = default;
    // Fwmass and Rwmass is total all wheels mass
    Car(float mass, float Fwmass, float Rwmass, float sf, float kf, float e, int wd_types = 0)
        : WD_types(wd_types), m_CG(0, 0), m_rotation(0)
    {

        m_wheels = std::vector<RigidBody>(2);
        initBase(mass, Fwmass, Rwmass, sf, kf, e, wd_types);
    }
    ~Car(){};

        void setPosition(float x, float y);

    void setPosition(const sf::Vector2f& position);

    void setRotation(float angle);

    void setScale(float factorX, float factorY);

    void setScale(const sf::Vector2f& factors);

    void setOrigin(float x, float y);

    void setOrigin(const sf::Vector2f& origin);

    const sf::Vector2f& getPosition() const;

    const sf::Vector2f& getPrePosition();

    float getRotation() const;

    const sf::Vector2f& getScale() const;

    const sf::Vector2f& getOrigin() const;

    void move(float offsetX, float offsetY);

    void move(const sf::Vector2f& offset);

    void rotate(float angle);

    void scale(float factorX, float factorY);

    void scale(const sf::Vector2f& factor);

    const sf::Transform& getTransform() const;

    const sf::Transform& getInverseTransform() const;

    const sf::Transform& getPreTransform() const;

    const sf::Transform& getPreInverseTransform() const;

    void initBase(float mass, float Fwmass, float Rwmass, float sf, float kf, float e, int wd_types = 0);
    void initdunebuggy(const sf::Texture &tww, const sf::Texture &tbod, const sf::Sprite &t1, const sf::Sprite &t2, const sf::Sprite &body);

    /*
    /// b and c is in percentange, like 80% or 60%, h is height of cg from wheels
    */
    void setWheelBase(float b, float c, float h);
    // physics, a car is considered like a multi-body system, so treat it as 1 object
    void applyGravity(float gravity);
    void addForce(sf::Vector2f &force);
    void addForceAtPos(sf::Vector2f &force, sf::Vector2f appliedpoint);
    void addTorque(float torque);

    void applyforcetorque(const sf::Vector2f &originpoint, const sf::Vector2f &appliedpoint);
    void applyforcetorque(const sf::Vector2f &force, const sf::Vector2f &originpoint, const sf::Vector2f &appliedpoint);
    void applyGraTorque();

    void springupdate();

    const sf::FloatRect &getGlobalBounds(){
        sf::FloatRect rect1 = m_carbody.getGlobalBounds();
        sf::FloatRect rect2 = m_wheels[0].getGlobalBounds();
        sf::FloatRect rect3 = m_wheels[1].getGlobalBounds();

        sf::FloatRect rect4 = rect2.getUnion(rect3);
        sf::FloatRect rect5 = rect1.getUnion(rect4);
        m_AABB = getTransform().transformRect(rect5);
        return m_AABB;
    }
    const sf::FloatRect& getSweptGlobalBounds(){
        sf::FloatRect rect1 = m_carbody.getGlobalBounds();
        sf::FloatRect rect2 = m_wheels[0].getGlobalBounds();
        sf::FloatRect rect3 = m_wheels[1].getGlobalBounds();

        sf::FloatRect rect4 = rect2.getUnion(rect3);
        sf::FloatRect rect5 = rect1.getUnion(rect4);
        sf::FloatRect rect6 = getPreTransform().transformRect(rect5);

        m_SweptAABB = getGlobalBounds();
        m_SweptAABB = m_SweptAABB.getUnion(rect6); // combine pre bound and current bound
        return m_SweptAABB;
    }
    bool contains(const sf::Vector2f &point);
    OBB getOBB(){
        float rotates = m_rotation;
        setRotation(0);

        sf::FloatRect rect5 = getGlobalBounds();
        setRotation(rotates);
        sf::FloatRect rect6 = getGlobalBounds();

        m_OBB = OBB(sf::Vector2f(rect5.getSize()), rect6.getPosition() + rect6.getSize()/2.f, rotates);
        m_OBB.setOrigin(rect5.getSize()/2.f);
        m_OBB.setFillColor(sf::Color(255, 255, 255, 0));
        m_OBB.setOutlineColor(sf::Color::Red);
        m_OBB.setOutlineThickness(5);

        return m_OBB;
    }

    void freezing(bool freeze){
        isFreeze = freeze;
    }

    void setCOG(const sf::Vector2f &cg);
    void setCOG(float x, float y); 

    void rotatewheel();

    sf::Vector2f projectilevec(const sf::Vector2f &a, LineSegment &line);
    void wheelcollider(RigidBody &body, LineSegment &line);
    void CollisionDetection(Map &map);
    void DiscreteCollisionDetection(Map &map,sf::RenderTarget &window, const sf::View &view);
    void ContinuousCollisionDetection(Map &map);

    void solvewheels(RigidBody &body, RigidBody &another);
    void ResolveCollision();

    
    /*
    /// viewcoord = window.mapPixelToCoords(sf::Vector2i(0, 0), view)
    */
    void movepiece();
    void accelerate();
    void update(float deltatime);
    void updateposition(float dt);
    void draw(sf::RenderWindow &window)
    {
        states.transform = getTransform();

        window.draw(m_carbody, states);
        window.draw(m_wheels[0], states);
        window.draw(m_wheels[1], states);


    }
    sf::Vector2f getMoveamt(){
        moveamt = m_velocity*dt;
        return moveamt;
    }
private:
    
    SpringJoint rear;
    SpringJoint front;

    sf::RectangleShape shape;
    sf::RenderStates states;
    // sf::Transform m_transform, m_inversetransform;

    sf::Vector2f m_contact, m_normal; // a car is only have 1 contact point, wheels or its body, if wheel and body touch then contact is wheel, not body, if only body then body
    sf::Texture tw;
    sf::Texture tbody;

    OBB m_OBB;
    sf::FloatRect m_AABB;
    sf::FloatRect m_SweptAABB;

    sf::Vector2f          m_origin = {0, 0};            //!< Origin of translation/rotation/scaling of the object
    sf::Vector2f          m_position = {0, 0};          //!< Position of the object in the 2D world
    float                 m_rotation;                   //!< Orientation of the object, in degrees
    sf::Vector2f          m_scale = {1, 1};             //!< Scale of the object
    mutable sf::Transform m_transform;                  //!< Combined transformation of the object
    mutable bool          m_transformNeedUpdate;        //!< Does the transform need to be recomputed?
    mutable sf::Transform m_inverseTransform;           //!< Combined transformation of the object
    mutable bool          m_inverseTransformNeedUpdate; //!< Does the transform need to be recomputed?

    sf::Vector2f m_preposition = {0, 0};      //!< Next frame Position of the object in the 2D world
    mutable sf::Transform m_pretransform;              //!< Next frame Combined transformation of the object
    mutable sf::Transform m_preinverseTransform;       //!< Next frame Combined transformation of the object
    float                 m_prerotation;                   //!< next frame Orientation of the object, in degrees


public:

    std::vector<RigidBody> m_wheels;
    RigidBody m_carbody;
    float m_inertia = 0, m_invinertia = 0, m_mass = 0, m_invmass = 0;
    float m_torque = 0, m_omega = 0, m_ang_accel = 0;

    sf::Vector2f m_velocity = {0, 0};
    sf::Vector2f m_force, m_accelerate, m_force_pos, m_force_torque, m_gravity;
    sf::Vector2f m_CG;
    sf::Vector2f moveamt;
    sf::Vector2f m_localspringA, m_localspringB;

    
    float rotateamt;

    bool collide = false, precollide = false;
    bool flying = true;
    bool isFreeze = false;

    // sf::RenderStates states = sf::RenderStates::Default;

    // sf::Transform m_transform, m_inversetransform;

    sm::sk sk;
    float dt;
};
// wtf 4 classes in 1 header, need separate in next update
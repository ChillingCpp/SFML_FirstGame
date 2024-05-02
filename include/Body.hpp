#pragma once
#include <Menu.hpp>
#include <vector>
#include <SFMLMath.hpp>

using Type = sf::Sprite::Type;
enum Bodytype
{
    static_body = 0,    /// static: zero mass, zero velocity, may be manually moved
    kinematic_body = 1, /// kinematic: zero mass, non-zero velocity set by user, moved by solver
    dynamic_body = 2    /// dynamic: positive mass (mass > 0), non-zero velocity determined by forces, moved by solver
};
// struct MassData
// {
//     float Mass, invMass;
//     sf::Vector2f COM; // center of mass of a body
//     // shift radius square = std::lengthsquare(COM - center)
//     float I, invI; // inertia around the centroid and its inverse
// };
struct BodyDef
{
    BodyDef()
    {
        position = sf::Vector2f(0.0f, 0.0f);
        angle = 0.0f;
        linearVelocity = sf::Vector2f(0.0f, 0.0f);
        angularVelocity = 0.0f;
        angularDamping = 0.0f;
        allowSleep = true;
        awake = true;
        fixedRotation = false;
        bullet = false;
        type = Bodytype::static_body;
        enabled = true;
        gravityScale = 1.0f;
        restitution = 0.5f;
        restitution_threshold = 1;
        mass = 1;
        staticFriction = 0.5f;
        DynamicFriction = 0.2f;
    }

    float mass; // the body has 1 unique fixture so do this.
    /// The body type: static, kinematic, or dynamic.
    /// Note: if a dynamic body would have zero mass, the mass is set to one.
    Bodytype type;

    /// The world position of the body. Avoid creating bodies at the origin
    /// since this can lead to many overlapping shapes.
    sf::Vector2f position;

    /// The world angle of the body in radians.
    float angle;

    /// The linear velocity of the body's origin in world co-ordinates.
    sf::Vector2f linearVelocity;

    /// The angular velocity of the body.
    float angularVelocity;


    /// Linear damping is use to reduce the linear velocity. The damping parameter
    /// can be larger than 1.0f but the damping effect becomes sensitive to the
    /// time step when the damping parameter is large.
    /// Units are 1/time
    // float linearDamping; // none needed because airdrag coefficient 

    /// Angular damping is use to reduce the angular velocity. The damping parameter
    /// can be larger than 1.0f but the damping effect becomes sensitive to the
    /// time step when the damping parameter is large.
    /// Units are 1/time, we cant use friciton spinning so damping instead
    float angularDamping;

    // friction
    float staticFriction;

    float DynamicFriction;
    
    // resitution for solver
    float restitution;

    float restitution_threshold; // if contact velocity < restitution_threshold then it will skip that contact point.

    /// Set this flag to false if this body should never fall asleep. Note that
    /// this increases CPU usage.
    bool allowSleep;

    /// Is this body initially awake or sleeping?
    bool awake;

    /// Should this body be prevented from rotating? Useful for characters.
    bool fixedRotation;

    /// Is this a fast moving body that should be prevented from tunneling through
    /// other moving bodies? Note that all bodies are prevented from tunneling through
    /// kinematic and static bodies. This setting is only considered on dynamic bodies.
    /// @warning You should use this flag sparingly since it increases processing time.
    bool bullet;

    /// Does this body start out enabled?
    bool enabled;

    /// Scale the gravity applied to this body.
    float gravityScale;
};
struct WheelDef : public BodyDef
{

    WheelDef() : BodyDef()
    {
        staticFriction = 1.5f; 
        DynamicFriction = 1.f; // rubber friction is special
        RollingResistance = 0.01f;
    }

    float RollingResistance;
    // no air resistance because its small
};
// ALL THOSE TRANSFORM FUNCTION IS SET IN METER, NOT IN PIXEL, IF SET TO PIXEL, YOU BODY WILL BIG LIKE A MOUNTAIN
class RigidBody : public sf::Drawable
{
public:
    enum Shape
    {
        circle = 0, // for inertia calculation
        box = 1
    };
    RigidBody()
    {
    }
    RigidBody(Type t, BodyDef &def) : m_omega(def.angularVelocity),
                                      m_velocity(def.linearVelocity),
                                      m_sfriction(def.staticFriction),
                                      m_kfriction(def.DynamicFriction),
                                      m_restitution(def.restitution),
                                      m_rotation(def.angle),
                                      m_angularDamping(def.angularDamping),
                                      m_threshold(def.restitution_threshold),
                                      type(t),
                                      btype(def.type),
                                      m_accelerate(0, 0),
                                      m_force(0, 0),
                                      m_invmass(def.mass > 0 ? 1.f/def.mass : 0.f),
                                      m_ang_accel(0),
                                      m_torque(0)

    {
        if (btype == Bodytype::static_body || btype == Bodytype::dynamic_body)
        {
            m_invmass = 0;
            m_invinertia = 0;
        }
        if (t == Type::circle)
            shape = Shape::circle;
        else
            shape = Shape::box; // convex and capsule using this inertia calculation
    }
    ~RigidBody() {}

    void setPosition(float x, float y);

    void setPosition(const sf::Vector2f &position);

    void setRotation(float angle);

    void setScale(float factorX, float factorY);

    void setScale(const sf::Vector2f &factors);

    /// setSize for sprite
    /// shortcut for setScaling to size
    /// \warning you must recall this after changing the source texture of the sprite
    /// \see setScale
    void setSize(float sizeX, float sizeY); // setSize is actually setScale

    /// \see setScale, setSize(float sizeX, float sizeY)
    void setSize(const sf::Vector2f &size); // setSize is actually setScale

    void setOrigin(float x, float y);

    void setOrigin(const sf::Vector2f &origin);

    const sf::Vector2f &getPosition() const;

    float getRotation() const;

    const sf::Vector2f &getScale() const;

    const sf::Vector2f &getOrigin() const;

    void move(float offsetX, float offsetY);

    void move(const sf::Vector2f &offset);

    void rotate(float angle);

    void scale(float factorX, float factorY);

    void scale(const sf::Vector2f &factor);

    const sf::Transform &getTransform() const;

    const sf::Transform &getInverseTransform() const;

    const sf::Transform &getDrawingTransform() const;

    const sf::Transform &getDrawingInverseTransform() const;

    // for those who dont want the constructor;
    void initBase(Type t, BodyDef &def);

/// massdata functions for calculation
    void setMassData(float mass); // mass, inertia

    void setTexture(const sf::Texture &texture, bool resetRect = false);

    void setTextureRect(const sf::IntRect &rect);

    sf::FloatRect getLocalBounds() const
    {
        sf::IntRect textureRect = m_sprite.getTextureRect();
        float width = static_cast<float>(std::abs(textureRect.width));
        float height = static_cast<float>(std::abs(textureRect.height));

        return sf::FloatRect(0.f, 0.f, width / sf::scale, height / sf::scale);
    }

    sf::FloatRect getGlobalBounds() const
    {
        return getTransform().transformRect(getLocalBounds());
    }

    void calculateBoundingbox()
    {
        if (type == Type::circle)
        {
            CircleBB.position = getPosition();
            CircleBB.radius = std::max(sf::Vector2f(getLocalBounds().getSize() * getScale()).x,
                                       sf::Vector2f(getLocalBounds().getSize() * getScale()).y);
        }

        OBB.size = getLocalBounds().getSize() * getScale();
        OBB.position = getPosition();
        OBB.m_rotation = getRotation();
    }
    const sf::FloatCircle &getCircle()
    {
        return CircleBB;
    }
    const sf::FloatObb &getOBB()
    {
        calculateBoundingbox();
        return OBB;
    }
    // physics1 : force, torque, gravity
    void applyGravity(float gravity);
    void addForce(sf::Vector2f &force);
    void addForceAtPos(sf::Vector2f &force, sf::Vector2f appliedpoint);
    void addTorque(float torque);

    void applyforcetorque(const sf::Vector2f &originpoint, const sf::Vector2f &appliedpoint);
    void applyforcetorque(const sf::Vector2f &force, const sf::Vector2f &originpoint, const sf::Vector2f &appliedpoint);
    
    // void runge_kutta_method(sf::Vector2f &velocity, float deltatime);
    void update(float deltatime);

    // deltatime here is different to deltatime step if using continuous collision detection
    void updatepos(float deltatime);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getDrawingTransform();
        target.draw(m_sprite, states);
    }

protected:
    sf::Sprite m_sprite;
    sf::Texture m_texture;

    sf::Vector2f nullvec = {0, 0};

    sf::Vector2f m_origin;                     //!< Origin of translation/rotation/scaling of the object
    sf::Vector2f m_position;                   //!< Position of the object in the 2D world
    float m_rotation;                          //!< Orientation of the object, in degrees
    sf::Vector2f m_scale = {1, 1};             //!< Scale of the object
    mutable sf::Transform m_transform;         //!< Combined transformation of the object
    mutable bool m_transformNeedUpdate;        //!< Does the transform need to be recomputed?
    mutable sf::Transform m_inverseTransform;  //!< Combined transformation of the object
    mutable bool m_inverseTransformNeedUpdate; //!< Does the transform need to be recomputed?

    mutable sf::Transform m_Drawingtransform;            //!< for drawing to screen
    mutable sf::Transform m_DrawinginverseTransform; //!< for drawing to screen

    mutable sf::Transform m_Prevtransform;        //!< for sweep aabb
    mutable sf::Transform m_PrevinverseTransform; //!< for sweep aabb

    float m_radius;

    float m_sfriction;
    float m_kfriction;
    float m_restitution;
    float m_threshold;

    float m_invinertia;
    float m_invmass;

    float m_torque;
    float m_omega;
    float m_ang_accel;
    float m_angularDamping;

    sf::Vector2f m_velocity;
    sf::Vector2f m_accelerate;
    sf::Vector2f m_force;
    sf::Vector2f m_force_torque;
    sf::Vector2f m_gravity;
    sf::Vector2f m_COM;
    sf::Vector2f m_size;
    sf::Vector2f m_center; // center of sprite after resize

    sf::Sprite::Type type;
    Shape shape;

    Bodytype btype;

    sf::FloatCircle CircleBB;
    sf::FloatObb OBB;

    /* generate point from texture:
    step 1 : getPosition
    step 2 : run nested 2 for loop; depend on origin we will with step around 5 to 10
    step 3 : getPixel coordinate and adding vertex with that position
    step 4 : using convexhull generate algorithm from points to generate convexhull
    */
};
// usage
/// bodyDef def
/// .... // change def variable;
/// RigidBody body(type, def);
/// body.setTexture(your_texture);
/// body.setCOM(center_of_mass);
/// body.setSize(size);
/// body.setMassData(def.mass); // if not have COM and size then inertia will be undefined
#include "Body.hpp"
#include <iostream>
void RigidBody::setPosition(float x, float y)
{

    m_position.x = x;
    m_position.y = y;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}

////////////////////////////////////////////////////////////
void RigidBody::setPosition(const sf::Vector2f &position)
{
    setPosition(position.x, position.y);
}

////////////////////////////////////////////////////////////
void RigidBody::setRotation(float angle)
{
    m_rotation = std::fmod(angle, 360.f);
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}

////////////////////////////////////////////////////////////
void RigidBody::setScale(float factorX, float factorY)
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}

////////////////////////////////////////////////////////////
void RigidBody::setScale(const sf::Vector2f &factors)
{
    setScale(factors.x, factors.y);
}
// set RigidBody size, size in meter, NOT PIXELS
void RigidBody::setSize(float sizeX, float sizeY)
{
    if (shape == Shape::circle)
        sizeX = sizeY;
    m_size = sf::Vector2f(sizeX, sizeY);
    setScale(sf::meter_to_pixel(m_size) / sf::Vector2f(m_texture.getSize()));
    m_radius = std::hypot(m_size.x, m_size.y);
    m_center = m_size / 2.f;
}

/// \see setScale, setSize(float sizeX, float sizeY)
void RigidBody::setSize(const sf::Vector2f &size)
{
    setSize(size.x, size.y);
}

////////////////////////////////////////////////////////////
void RigidBody::setOrigin(float x, float y)
{
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}

////////////////////////////////////////////////////////////
void RigidBody::setOrigin(const sf::Vector2f &origin)
{
    setOrigin(origin.x, origin.y);
}

////////////////////////////////////////////////////////////
const sf::Vector2f &RigidBody::getPosition() const
{
    return m_position;
}

////////////////////////////////////////////////////////////
float RigidBody::getRotation() const
{
    return m_rotation;
}

////////////////////////////////////////////////////////////
const sf::Vector2f &RigidBody::getScale() const
{
    return m_scale;
}

////////////////////////////////////////////////////////////
const sf::Vector2f &RigidBody::getOrigin() const
{
    return m_origin;
}

////////////////////////////////////////////////////////////
void RigidBody::move(float offsetX, float offsetY)
{
    setPosition(m_position.x + offsetX, m_position.y + offsetY);
}

////////////////////////////////////////////////////////////
void RigidBody::move(const sf::Vector2f &offset)
{
    setPosition(m_position.x + offset.x, m_position.y + offset.y);
}

////////////////////////////////////////////////////////////
void RigidBody::rotate(float angle)
{
    setRotation(m_rotation + angle);
}

////////////////////////////////////////////////////////////
void RigidBody::scale(float factorX, float factorY)
{
    setScale(m_scale.x * factorX, m_scale.y * factorY);
}

////////////////////////////////////////////////////////////
void RigidBody::scale(const sf::Vector2f &factor)
{
    setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}

////////////////////////////////////////////////////////////
const sf::Transform &RigidBody::getTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle = -m_rotation * 3.141592654f / 180.f;
        float cosine = std::cos(angle);
        float sine = std::sin(angle);
        float sxc = m_scale.x * cosine;
        float syc = m_scale.y * cosine;
        float sxs = m_scale.x * sine;
        float sys = m_scale.y * sine;
        float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

        float Dtx = -m_origin.x * sf::scale * sxc - m_origin.y * sys * sf::scale + m_position.x * sf::scale;
        float Dty = m_origin.x * sf::scale * sxs - m_origin.y * syc * sf::scale + m_position.y * sf::scale;

        m_transform = sf::Transform(sxc, sys, tx,
                                    -sxs, syc, ty,
                                    0.f, 0.f, 1.f);
        m_Drawingtransform = sf::Transform(sxc, sys, Dtx,
                                           -sxs, syc, Dty,
                                           0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}

////////////////////////////////////////////////////////////
const sf::Transform &RigidBody::getInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = getTransform().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}


////////////////////////////////////////////////////////////
const sf::Transform &RigidBody::getDrawingTransform() const
{
    float angle = -m_rotation * 3.141592654f / 180.f;
    float cosine = std::cos(angle);
    float sine = std::sin(angle);
    float sxc = m_scale.x * cosine;
    float syc = m_scale.y * cosine;
    float sxs = m_scale.x * sine;
    float sys = m_scale.y * sine;
    float Dtx = (-m_origin.x * sf::scale * sxc) - (m_origin.y * sys * sf::scale) + m_position.x * sf::scale;
    float Dty = (m_origin.x * sf::scale * sxs) - (m_origin.y * syc * sf::scale) + m_position.y * sf::scale;

    m_Drawingtransform = sf::Transform(sxc, sys, Dtx,
                                       -sxs, syc, Dty,
                                       0.f, 0.f, 1.f);
    return m_Drawingtransform;
}


////////////////////////////////////////////////////////////
const sf::Transform &RigidBody::getDrawingInverseTransform() const
{
    m_DrawinginverseTransform = getDrawingTransform().getInverse();
    return m_DrawinginverseTransform;
}
void RigidBody::initBase(Type t, BodyDef &def)
{
    m_sfriction = def.staticFriction;
    m_kfriction = def.DynamicFriction;
    m_restitution = def.restitution;
    m_rotation = def.angle;
    m_angularDamping = def.angularDamping;
    m_velocity = def.linearVelocity;
    m_omega = def.angularVelocity;
    m_threshold = def.restitution_threshold;
    m_invmass = def.mass > 0 ? 1.f /def.mass : 0.f;
    btype = def.type;
    type = t;
    m_accelerate = nullvec;
    m_force = nullvec;
    m_invmass = 0;
    m_ang_accel = 0;
    m_torque = 0;
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
void RigidBody::setMassData(float mass)
{
    if (!m_sprite.getTexture()) return; // the sprite has no texture
    if (btype == Bodytype::static_body || btype == Bodytype::dynamic_body)
    {
        m_invmass = 0;
        m_invinertia = 0;
        return;
    }
    m_invmass = mass > 0 ? 1.f / mass : 0;
    switch (shape)
    {
    case circle:
        float inertia = 0.5f * mass * m_radius * m_radius + mass * sm::distancesqr(m_center, m_COM);
        m_invinertia = inertia > 0 ? 1.f / inertia : 0;
        break;
    case box:
        float k = 1.f / 12.f;
        float inertia = k * mass * m_radius * m_radius + mass * sm::distancesqr(m_center, m_COM);
        m_invinertia = inertia > 0 ? 1.f / inertia : 0;
        break;
    default:
        break;
    }
}
void RigidBody::setTexture(const sf::Texture &texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no valid texture & rect before
    m_texture = texture;
    m_sprite.setTexture(m_texture, resetRect);
    if (m_size == nullvec)
        m_size = sf::pixel_to_meter(sf::Vector2f(m_texture.getSize()));
    setSize(m_size);
    
    // Assign the new texture
}
void RigidBody::setTextureRect(const sf::IntRect &rect)
{
    m_sprite.setTextureRect(rect);
}
void RigidBody::applyGravity(float gravity)
{
    m_gravity = {0, gravity};
}
void RigidBody::addForce(sf::Vector2f &force)
{
    // this will add force to center of mass of the object
    m_force += force;
}
void RigidBody::addForceAtPos(sf::Vector2f &force, sf::Vector2f appliedpoint)
{
    // addforce to any point in local object position
    m_force_torque += force;
    sf::Vector2f origin = getPosition();
    applyforcetorque(m_force_torque, origin, appliedpoint);

    addForce(force);
}
void RigidBody::addTorque(float torque)
{
    m_torque += torque;
}
void RigidBody::applyforcetorque(const sf::Vector2f &object_pos, const sf::Vector2f &appliedpoint)
{
    // addforce to center of mass, if originpoint equal to CoM then no torque, originpoint is usually contact point.
    applyforcetorque(m_force, object_pos, appliedpoint);
}
void RigidBody::applyforcetorque(const sf::Vector2f &force, const sf::Vector2f &object_pos, const sf::Vector2f &appliedpoint)
{
    sf::Vector2f lever_arm = appliedpoint - object_pos;
    float torque = sm::cross(lever_arm, force);
    addTorque(torque);
}

// void RigidBody::runge_kutta_method(sf::Vector2f &velocity, float deltatime)
// {
//     sf::Vector2f dx, dx1, dx2, dx3, dx4, dv, dv1, dv2, dv3, dv4;
//     sf::Vector2f v0 = velocity;
//     float dt = deltatime;

//     dx1 = velocity * dt;
//     dv1 = calcAcceleration(velocity) * dt;

//     dx2 = (velocity + dv1 / 2.f) * dt;
//     dv2 = (calcAcceleration(dv1) + calcAcceleration(dv1) * 0.5f) * dt;

//     dx3 = (velocity + dv2 / 2.f) * dt;
//     dv3 = (calcAcceleration(dv2) + calcAcceleration(dv2) * 0.5f) * dt;

//     dx4 = (velocity + dv3 / 2.f) * dt;
//     dv4 = (calcAcceleration(dv3) + calcAcceleration(dv3) * 0.5f) * dt;

//     dx = (dx1 + dx2 * 2.f + dx3 * 2.f + dx4) / 6.f; // new delta position
//     dv = (dv1 + dv2 * 2.f + dv3 * 2.f + dv4) / 6.f; // new delta velocity

//     move(dx);
//     m_velocity += dv;
// }
void RigidBody::update(float deltatime)
{
    if (btype == Bodytype::static_body || btype == Bodytype::dynamic_body)
        return;
    // applyGraTorque();
    m_ang_accel = m_torque * m_invinertia;
    m_omega += m_ang_accel * deltatime;

    m_accelerate = (m_force)*m_invmass;
    m_velocity += m_accelerate * deltatime;
}
// deltatime here is different than deltatime step if  using continuous collision detection
void RigidBody::updatepos(float deltatime)
{
    if (btype == Bodytype::static_body || btype == Bodytype::dynamic_body)
        return;
    rotate(sm::radToDeg(m_omega * deltatime));
    move(m_velocity * deltatime);

    m_force = {};
    m_force_torque = {};
    m_torque = 0;
}
#include <Car.hpp>
#include <cmath>
#include <VertexDeque.hpp>

void Car::setPosition(float x, float y)
{

    
    m_position.x = x;
    m_position.y = y;
    m_preposition = m_position + m_velocity*dt;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Car::setPosition(const sf::Vector2f& position)
{
    setPosition(position.x, position.y);
}


////////////////////////////////////////////////////////////
void Car::setRotation(float angle)
{
    m_rotation = std::fmod(angle, 360.f);
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Car::setScale(float factorX, float factorY)
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Car::setScale(const sf::Vector2f& factors)
{
    setScale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
void Car::setOrigin(float x, float y)
{
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Car::setOrigin(const sf::Vector2f& origin)
{
    setOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
const sf::Vector2f& Car::getPosition() const
{
    return m_position;
}

const sf::Vector2f& Car::getPrePosition()
{
    m_preposition = m_position + m_velocity*dt;
    return m_preposition;
}

////////////////////////////////////////////////////////////
float Car::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const sf::Vector2f& Car::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
const sf::Vector2f& Car::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
void Car::move(float offsetX, float offsetY)
{
    setPosition(m_position.x + offsetX, m_position.y + offsetY);
}


////////////////////////////////////////////////////////////
void Car::move(const sf::Vector2f& offset)
{
    setPosition(m_position.x + offset.x, m_position.y + offset.y);
}


////////////////////////////////////////////////////////////
void Car::rotate(float angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void Car::scale(float factorX, float factorY)
{
    setScale(m_scale.x * factorX, m_scale.y * factorY);
}


////////////////////////////////////////////////////////////
void Car::scale(const sf::Vector2f& factor)
{
    setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}


////////////////////////////////////////////////////////////
const sf::Transform& Car::getTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * 3.141592654f / 180.f;
        float cosine = std::cos(angle);
        float sine   = std::sin(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

        m_transform = sf::Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const sf::Transform& Car::getInverseTransform() const
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
const sf::Transform& Car::getPreTransform() const
{
        float angle  = -m_prerotation * 3.141592654f / 180.f;
        float cosine = std::cos(angle);
        float sine   = std::sin(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_preposition.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_preposition.y;

        m_pretransform = sf::Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
    return m_pretransform;
}


////////////////////////////////////////////////////////////
const sf::Transform& Car::getPreInverseTransform() const
{
    m_preinverseTransform = getPreTransform().getInverse();
    return m_preinverseTransform;
}
void Car::initBase(float mass, float Fwmass, float Rwmass, float sf, float kf, float e, int wd_types)
{
    m_wheels[0].initBase(RigidBody::Shape::circle, sf, kf, e, Rwmass);
    m_wheels[1].initBase(RigidBody::Shape::circle, sf, kf, e, Fwmass);

    m_carbody.initBase(RigidBody::Shape::box, 0.8, 0.5, 0.3, mass - Rwmass - Fwmass);
    m_CG = {0, 0};
    m_mass = mass;
    m_invmass = 1.f / m_mass;
}
void Car::initdunebuggy(const sf::Texture &tww, const sf::Texture &tbod, const sf::Sprite &t1, const sf::Sprite &t2, const sf::Sprite &body)
{

    this->tbody = tbod;
    this->tw = tww;

    m_carbody.setSprite(body, tbody);
    m_wheels[0].setSprite(t1, tw);
    m_wheels[1].setSprite(t2, tw);

    m_inertia = m_wheels[0].m_inertia + m_wheels[1].m_inertia + m_carbody.m_inertia;
    m_invinertia = 1.f / m_inertia;

    rear = SpringJoint(m_wheels[0].getPosition(), m_wheels[0].getPosition() + sf::Vector2f(20, -15));
    front = SpringJoint(m_wheels[1].getPosition(), m_wheels[1].getPosition() + sf::Vector2f(-20, -15));
    rear.setBody(&m_wheels[0], &m_carbody);
    front.setBody(&m_wheels[1], &m_carbody);

    rear.m_localstart = rear.start;
    rear.m_localend = rear.end;

    front.m_localstart = front.start;
    front.m_localend = front.end;

    rear.setConstantA(12000, 1000);
    front.setConstantA(12000, 1000);

    rear.setConstantB(50000, 3500);
    front.setConstantB(50000, 3500);

    setOrigin(getGlobalBounds().getSize() / 2.f - m_carbody.getOrigin());
}
void Car::setWheelBase(float b, float c, float h)
{
    float L = std::abs(m_wheels[0].getPosition().x - m_wheels[1].getPosition().x);
    sf::Vector2f wheelocal = getInverseTransform().transformPoint(m_wheels[0].getPosition());
    m_CG = {wheelocal.x + c * L, h};
    m_inertia += sk.getLengthsqr(m_CG - getOrigin()) * m_mass;
    m_invinertia = 1.f / m_inertia;
    std::cout << m_invinertia << '\n';
}
void Car::applyGravity(float gravity)
{
    m_gravity = {0, gravity};
    rear.setGravity(m_gravity);
    front.setGravity(m_gravity);
}
void Car::addForce(sf::Vector2f &force)
{
    // this will add force to center of mass of the car
    m_force += force;
    applyforcetorque(getPosition(), m_contact);
}

void Car::addForceAtPos(sf::Vector2f &force, sf::Vector2f appliedpoint)
{
    // addforce to any point in local car position
    m_force_torque += force;
    sf::Vector2f origin = contains(m_contact) ? m_contact : getPosition();
    applyforcetorque(m_force_torque, origin, appliedpoint);

    // sf::Vector2f origin1 = m_wheels[0].getGlobalBounds().contains(m_wheels[0].m_contact) ? m_wheels[0].m_contact : m_wheels[0].getPosition();
    // m_wheels[0].applyforcetorque(m_force_torque, origin1, appliedpoint);
    m_wheels[0].addForceAtPos(force, appliedpoint);

    // sf::Vector2f origin2 = m_wheels[1].getGlobalBounds().contains(m_wheels[1].m_contact) ? m_wheels[1].m_contact : m_wheels[1].getPosition();
    // m_wheels[1].applyforcetorque(m_force_torque, origin2, appliedpoint);

    sf::Vector2f radius = origin - appliedpoint;
    float cosine = sk.getcosinevalue(force, radius);
    m_force_pos += force * cosine;
}
void Car::addTorque(float torque)
{
    m_torque += torque;
}
void Car::applyforcetorque(const sf::Vector2f &object_pos, const sf::Vector2f &appliedpoint)
{
    // addforce to center of mass, if originpoint equal to CoM then no torque, originpoint is usually contact point.
    applyforcetorque(m_force, object_pos, appliedpoint);
}
void Car::applyforcetorque(const sf::Vector2f &force, const sf::Vector2f &object_pos, const sf::Vector2f &appliedpoint)
{
    sf::Vector2f lever_arm =  appliedpoint - object_pos;
    float torque = sk.cross(lever_arm, force);
    addTorque(torque);
}

void Car::applyGraTorque()
{
        // sf::Vector2f rotary0 = getGlobalBounds().contains(m_wheels[0].m_contact) ? m_wheels[0].m_contact : getTransform()*m_CG;
        // sf::Vector2f rotary1 = getGlobalBounds().contains(m_wheels[1].m_contact) ? m_wheels[1].m_contact : getTransform()*m_CG;

        sf::Vector2f graForce = m_gravity*m_mass;
        // applyforcetorque(graForce, rotary0, getTransform()*m_CG);
        // applyforcetorque(graForce, rotary1, getTransform()*m_CG);

        // printv(rotary0);
        // printv(rotary1);
}

bool Car::contains(const sf::Vector2f &point)
{
    return m_carbody.getGlobalBounds().contains(point) || m_wheels[0].getGlobalBounds().contains(point) || m_wheels[1].getGlobalBounds().contains(point);
}
void Car::setCOG(const sf::Vector2f &cg)
{
    setCOG(cg.x, cg.y);
}
void Car::setCOG(float x, float y)
{
    m_CG = {x, y};
}

sf::Vector2f Car::projectilevec(const sf::Vector2f &a, LineSegment &line)
{
    const sf::Vector2f B = line.start;

    const sf::Vector2f BA = a - B;
    const sf::Vector2f BC = line.vector();

    sf::Vector2f D = B + sk.projection(BA, BC);
    return a - D;
}
void Car::wheelcollider(RigidBody &body, LineSegment &line)
{
    const float radius = body.m_radius;
    const sf::Vector2f a = getTransform() * body.getPosition();

    const sf::Vector2f DA = projectilevec(a, line);

    float dist = sk.getLength(DA);
    std::cout << dist << '\n';

    if (dist > radius)
    {
        // body.m_penetration = 0;
        // body.m_contact = body.m_normal = {};
        return;
    }

    sf::Vector2f m_normal = sk.getNormalized(DA);
    // body.m_normal = m_normal;

    // compute intersect points
    if (dist <= radius)
    {
        flying = false;
        // body.m_contact = (a - DA);
        // body.m_penetration = radius - dist;
        return;
    }

    return;
}
void Car::CollisionDetection(Map &map)
{
}
void Car::DiscreteCollisionDetection(Map &map, sf::RenderTarget &window, const sf::View &view)
{
    
    
}
void Car::ContinuousCollisionDetection(Map &map)
{
}
void Car::solvewheels(RigidBody &body, RigidBody &ano)
{

    // move(body.m_penetration * body.m_normal);
    float e = body.m_restitution;
    float sf = (0.5f + 0.6f) * 0.5f;
    float kf = (0.5f + 0.45f) * 0.5f;

    sf::Vector2f nullvec = {0, 0};

    // first separate our object

    // sf::Vector2f ra = body.m_contact - getTransform() * body.getPosition();
    // sf::Vector2f rb = body.m_contact - getTransform() * getOrigin();
    // std::cout << "ra\n";
    // printv(ra);

    // sf::Vector2f raPerp = sf::Vector2f(-ra.y, ra.x);
    // sf::Vector2f rbPerp = sf::Vector2f(-rb.y, rb.x);

    // sf::Vector2f ang_velA = raPerp * body.m_omega;
    // sf::Vector2f ang_velB = rbPerp * m_omega;

    // float relavel = sk.getLength(body.m_velocity) - sk.getLength(m_velocity);

    // sf::Vector2f vel1 = relavel > 0 ? sk.getNormalized(body.m_velocity) * relavel : m_velocity;

    // sf::Vector2f relativeVelocity = vel1 + ang_velB;

    // float contactVelocityMag = sk.dot(relativeVelocity, body.m_normal);
    // if (contactVelocityMag > 0.f)
    // {
    //     return;
    // }

    // float raPerpdotN = sk.dot(raPerp, body.m_normal);
    // float rbPerpdotN = sk.dot(rbPerp, body.m_normal);

    // float denom = m_invmass + (raPerpdotN * raPerpdotN) * m_invinertia;

    // float j = -(1.f + e) * contactVelocityMag;
    // j /= denom;

    // sf::Vector2f impulse = j * body.m_normal;

    // // friction
    // sf::Vector2f tangent = relativeVelocity - sk.dot(relativeVelocity, body.m_normal) * body.m_normal;
    // if (sk.nearlyequal(tangent, nullvec))
    // {
    //     tangent = nullvec;
    // }
    // else
    // {
    //     tangent = sk.normalize(tangent);
    // }
    // float raPerpdotT = sk.dot(raPerp, tangent);
    // float rbPerpdotT = sk.dot(rbPerp, tangent);

    // float denom1 = m_invmass + (raPerpdotT * raPerpdotT) * m_invinertia;
    // float jt = -sk.dot(relativeVelocity, tangent);
    // jt /= denom1;

    // sf::Vector2f frictionImpulse = nullvec;

    // if (std::abs(jt) <= j * sf)
    // {
    //     frictionImpulse = jt * tangent;
    // }
    // else
    // {
    //     frictionImpulse = -j * tangent * kf;
    // }
    // if (sk.nearlyequal(frictionImpulse))
    //     frictionImpulse = nullvec;

    // body.m_velocity += (impulse + frictionImpulse) * m_invmass;
    // body.m_omega += (sk.cross(ra, impulse) + sk.cross(ra, frictionImpulse)) * m_invinertia;

    // m_velocity += (impulse + frictionImpulse) * m_invmass;
    // m_omega += (sk.cross(rb, impulse) + sk.cross(rb, frictionImpulse)) * (m_invinertia);
    // body.m_velocity = {};
    // ano.m_velocity = {};
    // m_carbody.m_velocity = {};
    // m_velocity = {};
    // m_gravity = {};
    std::cout << "\n\n";
}
void Car::ResolveCollision()
{
    // if (m_wheels[0].m_contact != sf::Vector2f(0, 0))
    // {
    //     solvewheels(m_wheels[0], m_wheels[1]);
    // }
    // if (m_wheels[1].m_contact != sf::Vector2f(0, 0))
    // {
    //     solvewheels(m_wheels[1], m_wheels[0]);

    // }
    springupdate();
}
void Car::springupdate()
{
    // rear.start = (getTransform() * m_wheels[0].getPosition()); // always
    // rear.end = m_carbody.getTransform() * getTransform() * rear.m_localend;
    // rear.setAnchorA(getTransform() * rear.m_localstart);
    // rear.setAnchorB(getTransform() * rear.m_localend);

    // front.start = (getTransform() * m_wheels[1].getPosition()); // always
    // front.end = m_carbody.getTransform() * getTransform() * front.m_localend;
    // front.setAnchorA(getTransform() * front.m_localstart);
    // front.setAnchorB(getTransform() * front.m_localend);

    // rear.solveposition(dt);
    // front.solveposition(dt);
}
void Car::movepiece()
{
    float relavel1 = sk.getLength(m_wheels[0].m_velocity) - sk.getLength(m_velocity);
    float relavel2 = sk.getLength(m_wheels[1].m_velocity) - sk.getLength(m_velocity);
    float relavel3 = sk.getLength(m_carbody.m_velocity) - sk.getLength(m_velocity);

    sf::Vector2f vel1 = relavel1 > 0 ? sk.getNormalized(m_wheels[0].m_velocity) * relavel1 : sf::Vector2f(0, 0);
    sf::Vector2f vel2 = relavel2 > 0 ? sk.getNormalized(m_wheels[1].m_velocity) * relavel2 : sf::Vector2f(0, 0);
    sf::Vector2f vel3 = relavel3 > 0 ? sk.getNormalized(m_carbody.m_velocity) * relavel3 : sf::Vector2f(0, 0);

    m_wheels[0].move(vel1 * dt);
    m_wheels[1].move(vel2 * dt);
    m_carbody.move(vel3 * dt);
    rear.movea(vel1*dt, vel1*dt);
    front.movea(vel2*dt, vel2*dt);
}
void Car::update(float deltatime)
{
    applyGraTorque();
    dt = deltatime;

    sf::Vector2f accelamt = sk.round((m_force + m_force_pos) * m_invmass + m_gravity * dt);
    m_wheels[0].m_velocity += accelamt;
    m_wheels[1].m_velocity += accelamt;
    m_carbody.m_velocity += accelamt;
    m_velocity += accelamt;

    // printv(m_velocity);
    // printv(m_wheels[0].m_velocity);
    m_omega += (m_torque * m_invinertia) * dt;


    m_preposition = m_position + m_velocity*dt;
    m_prerotation = m_rotation + m_omega*dt;


    // m_wheels[0].m_contact = {};
    // m_wheels[1].m_contact = {};
}
void Car::updateposition(float dt){
    if (isFreeze){
        m_velocity = {};
        m_accelerate = {};
        m_ang_accel = 0;
        m_omega = 0;
        m_carbody.m_velocity = {};
        m_wheels[0].m_velocity = {};
        m_wheels[1].m_velocity = {};
        m_gravity = {};
        return;
    }
    moveamt = m_velocity * dt;
    rotateamt = m_omega * dt;

    move(moveamt);
    movepiece();
    rotate(sk.radToDeg(rotateamt));
    m_wheels[0].rotate(sk.radToDeg(m_wheels[0].m_omega * dt));
    m_wheels[1].rotate(sk.radToDeg(m_wheels[1].m_omega * dt));

    m_force = m_force_pos = m_force_torque = {};
    m_torque = 0;
    m_contact = {};
}

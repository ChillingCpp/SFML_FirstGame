// #pragma once
// #include <SFML/Graphics/Shape.hpp>
// #include <SFML/Graphics/LineSegment.hpp>
// #include <SFML/Graphics/Sprite.hpp>
// #include <SFML/Graphics/Collisions.hpp>
// #include <Body.hpp>

// #define _lengthUnitsPerMeter 1.0f
// typedef signed char int8;
// typedef signed short int16;
// typedef signed int int32;
// typedef unsigned char uint8;
// typedef unsigned short uint16;
// typedef unsigned int uint32;

// // NOT USED

// /// This holds contact filtering data.
// /// A fixture definition is used to create a fixture. This class defines an
// /// abstract fixture definition. You can reuse fixture definitions safely.
// class Body;
// class Fixture;
// enum Type{
//     circle =0,
//     box = 1,
//     convex = 2
// };
// struct FixtureDef
// {
//     /// The constructor sets the default fixture definition values.
//     FixtureDef()
//     {
//         // sprite = nullptr; 
//         friction = 0.2f;
//         restitution = 0.0f;
//         restitutionThreshold = 1.0f;
//         density = 0.0f;
//         isSensor = false;
//         // UseSprite = false;
//     }

//     /// The shape, this must be set. The shape will be cloned, so you
//     /// can create the shape on the stack.
//     // const sf::Sprite *sprite; // for those who dont use shapes instead a sprite for convenient

//     /// The friction coefficient, usually in the range [0,1].
//     float friction;

//     /// The restitution (elasticity) usually in the range [0,1].
//     float restitution;

//     /// Restitution velocity threshold, usually in m/s. Collisions above this
//     /// speed have restitution applied (will bounce).
//     float restitutionThreshold;

//     /// The density, usually in kg/m^2.
//     float density;

//     /// A sensor shape collects contact information but never generates a collision
//     /// response.
//     bool isSensor;
// };
// class Fixture
// {
// public:

//     /// Get the child shape. You can modify the child shape, however you should not change the
//     /// number of vertices because this will crash some collision caching mechanisms.
//     /// Manipulating the shape may lead to non-physical behavior.
//     sf::Sprite* getSprite()
//     {
//         // return m_sprite;
//     }
//     const sf::Sprite* getSprite() const
//     {
//         // return m_sprite;
//     }

//     /// Set if this fixture is a sensor.
//     void SetSensor(bool sensor)
//     {
//         m_isSensor = sensor;
//     }

//     /// Is this fixture a sensor (non-solid)? // là vật mềm
//     /// @return the true if the shape is a sensor.
//     bool IsSensor() const
//     {
//         return m_isSensor;
//     }

//     /// Get the next fixture in the parent body's fixture list.
//     /// @return the next shape.
//     Fixture *GetNext();
//     const Fixture *GetNext() const;

//     /// Test a point for containment in this fixture.
//     /// @param p a point in world coordinates.
//     bool TestPoint(const sf::Vector2f &p) const;

//     /// Cast a ray against this shape.
//     /// @param output the ray-cast results.
//     /// @param input the ray-cast input parameters.
//     /// @param childIndex the child shape index (e.g. edge index)
//     bool RayCast(RayCastOutput *output, const RayCastInput &input, int32 childIndex) const;

//     /// Set the density of this fixture. This will _not_ automatically adjust the mass
//     /// of the body. You must call Body::ResetMassData to update the body's mass.
//     void SetDensity(float density)
//     {
//         m_density = density;
//     }

//     /// Get the density of this fixture.
//     float GetDensity() const
//     {
//         return m_density;
//     }

//     /// Get the coefficient of friction.
//     float GetFriction() const
//     {
//         return m_friction;
//     }

//     /// Set the coefficient of friction. This will _not_ change the friction of
//     /// existing contacts.
//     void SetFriction(float friction)
//     {
//         m_friction = friction;
//     }

//     /// Get the coefficient of restitution.
//     float GetRestitution() const
//     {
//         return m_restitution;
//     }

//     /// Set the coefficient of restitution. This will _not_ change the restitution of
//     /// existing contacts.
//     void SetRestitution(float restitution)
//     {
//         m_restitution = restitution;
//     }

//     /// Get the restitution velocity threshold.
//     float GetRestitutionThreshold() const
//     {
//         return m_restitutionThreshold;
//     }

//     /// Set the restitution threshold. This will _not_ change the restitution threshold of
//     /// existing contacts.
//     void SetRestitutionThreshold(float threshold)
//     {
//         m_restitutionThreshold = threshold;
//     }

//     /// Get the fixture's AABB. This AABB may be enlarge and/or stale.
//     /// If you need a more accurate AABB, compute it using the shape and
//     /// the body transform.
//     const sf::FloatRect &GetAABB(int32 childindex) const
//     {
//     }

// public:
//     Fixture();

//     void Create(const FixtureDef &def)
//     {
//         m_density  = def.density;
//         m_friction = def.friction;
//         m_restitution = def.restitution;
//         m_restitutionThreshold = def.restitutionThreshold;
//         m_isSensor = def.isSensor;
//         m_UseSprite = def.UseSprite;

//     }
//     void Destroy(){
//         // delete m_sprite;
//         delete m_next;

//     }
//     sf::Sprite* Clone(const sf::Sprite *s){
//         sf::Sprite *sprite = new sf::Sprite();
//         *sprite = *s;
//         return sprite;
//     }

//     // We need separation create/destroy functions from the constructor/destructor because
//     // the destructor cannot access the allocator (no destructor arguments allowed by C++).

//     float m_density;

//     Fixture *m_next;

//     // BoundingBox box;

//     float m_friction;
//     float m_restitution;
//     float m_restitutionThreshold;

//     bool m_isSensor;
//     bool m_UseSprite;
// };

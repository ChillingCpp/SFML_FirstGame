#pragma once
#include <Body.hpp>
#include <Map.hpp>
struct Contact
{
    sf::Vector2f contact;
    sf::Vector2f normal;
    float penetration_depth;
    float time_of_collision;
};
struct ConstrainSolver{
    float normalImpulse;
    float tangentImpulse;
    float friction;
    float restitution;
};
struct Manifold{

    RigidBody BodyA;
    RigidBody BodyB;
    std::vector<Contact> contact = std::vector<Contact>(2);
    int contactcount;
};
// Manifold for Vehicle
class CarManifold
{
public:
    CarManifold();
    ~CarManifold();
public:
    std::vector<Contact> Frontcontact;
    std::vector<Contact> Rearcontact;
    std::vector<Contact> bodycontact;

    std::pair<int, int> pair1;
    std::pair<int, int> pair2;
    std::pair<int, int> pair3;

    
    int Frontcontactcount;
    int Rearcontactcount;
    int bodycontactcount;
private:
    
    
};
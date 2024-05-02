#pragma once

#include <Car.hpp>
#include <Manifold.hpp>
class World
{
public:
    World();
    void initTexture()
    {
        if (!tbody.loadFromFile("D:/sfmlcpp/res/Car/vehicles-dunebuggy.png"))
        {
        }
        if (!twheel.loadFromFile("D:/sfmlcpp/res/Car/wheel.png"))
        {
        }
        tbody.setSmooth(true);
        twheel.setSmooth(true);
    }
    void initall(float step, int size)
    {

        m_stage.initMap(step, size);
        sf::Sprite carbody, cartire1, cartire2;

        initTexture();
        carbody.setTexture(tbody);
        cartire1.setTexture(twheel);
        cartire2.setTexture(twheel);

        cartire1.setOrigin(sf::Vector2f(twheel.getSize() / (unsigned)2));
        cartire1.setScale((wheelradius * 2) / twheel.getSize().x, (wheelradius * 2) / twheel.getSize().y);
        cartire1.setPosition(carbody.getTransform().transformPoint(sf::Vector2f(33, 71)));

        cartire2.setOrigin(sf::Vector2f(twheel.getSize() / (unsigned)2));
        cartire2.setScale((wheelradius * 2) / twheel.getSize().x, (wheelradius * 2) / twheel.getSize().y);
        cartire2.setPosition(carbody.getTransform().transformPoint(sf::Vector2f(200, 71)));

        m_dunebuggy = Car(2000, 100, 100, 0.9, 0.65, 0.0001);
        m_dunebuggy.initdunebuggy(twheel, tbody, cartire1, cartire2, carbody);
        m_dunebuggy.setWheelBase(0.65, 0.35, wheelradius + 5);
        m_dunebuggy.setPosition(350, 200);
        m_dunebuggy.applyGravity(900);
        m_substep = 10; // minimum
    }
    // setsubstep for more accurately calculate
    void setSubsteps(float substep)
    {
        m_substep = substep;
    }
    int floori(float var, int decplace)
    {
        return std::floor(var / std::pow(10.f, decplace)) * std::pow(10.f, decplace);
    }
    std::pair<int, int> findrangecollsion(sf::FloatRect &rect, sf::Vector2f &viewcoord)
    {

        float step = m_stage.getStep();
        std::vector<float> realposX;
        realposX.push_back(rect.left);
        realposX.push_back(rect.left + rect.width);
        float x = MAP_ADD_SIZE / step * 2.f; // plus 1 because map[1] is actually real coordinate
        // solved when plus 1 in Map.hpp function operator[]

        // realposX = obj.position.x - window.mapPixeltoCoords(sf::Vector2o(0, 0), view).x;
        int startpos = x + floori(std::ceil(realposX[0]), 1) * (2.f / step);
        int endpos = x + floori(std::ceil(realposX[1]), 1) * (2.f / step);
        return {startpos, endpos}; // hàm tìm vị trí mảng mà vật đang ở (tác dụng không ổn, cần cải thiện (camera minecraft))
    }

    std::pair<int, int> findrangecollsion(RigidBody &body, sf::Vector2f &viewcoord)
    {

        float step = m_stage.getStep();
        std::vector<float> realposX;
        sf::Vector2f bodypos = m_dunebuggy.getTransform() * body.getPosition();
        realposX.push_back(bodypos.x - body.getGlobalBounds().width / 2 - viewcoord.x);
        realposX.push_back(bodypos.x + body.getGlobalBounds().width / 2 - viewcoord.x);
        float x = MAP_ADD_SIZE * (2.f / step); // plus 1 because map[1] is actually real coordinate
        // solved when plus 1 in Map.hpp function operator[]

        // realposX = obj.position.x - window.mapPixeltoCoords(sf::Vector2o(0, 0), view).x;
        int startpos = x + floori(std::ceil(realposX[0]), 1) * (2.f / step);
        int endpos = x + floori(std::ceil(realposX[1]), 1) * (2.f / step);
        return {startpos, endpos}; // hàm tìm vị trí mảng mà vật đang ở (tác dụng không ổn, cần cải thiện (camera minecraft))
    }
    std::pair<int, int> findcarcollsion(sf::Vector2f &viewcoord)
    {

        float step = m_stage.getStep();
        std::vector<float> realposX;
        sf::Vector2f bodypos = m_dunebuggy.getPosition();
        realposX.push_back(bodypos.x - m_dunebuggy.getGlobalBounds().width / 2 - viewcoord.x);
        realposX.push_back(bodypos.x + m_dunebuggy.getGlobalBounds().width / 2 - viewcoord.x);
        float x = MAP_ADD_SIZE / step * 2.f; // plus 1 because map[1] is actually real coordinate
        // solved when plus 1 in Map.hpp function operator[]

        // realposX = obj.position.x - window.mapPixeltoCoords(sf::Vector2o(0, 0), view).x;
        int startpos = x + floori(std::ceil(realposX[0]), 1) * (2.f / step);
        int endpos = x + floori(std::ceil(realposX[1]), 1) * (2.f / step);
        return {startpos, endpos}; // hàm tìm vị trí mảng mà vật đang ở (tác dụng không ổn, cần cải thiện (camera minecraft))
    }

    sf::Vector2f closetPointonSegment(const sf::Vector2f &center, LineSegment &line)
    {
        const sf::Vector2f B = line.start;

        const sf::Vector2f BA = center - B;
        const sf::Vector2f BC = line.vector();

        float t = math.dot(BA, BC) / math.dot(BC, BC);
        if (t < 0)
            return line.start;
        if (t > 1)
            return line.end;

        return B + t * BC;
    }
    const sf::Vector2f ProjectVecOnLine(const sf::Vector2f &center, LineSegment &line)
    {
        const sf::Vector2f linetocenter = center - line.start;
        const sf::Vector2f linevector = line.end - line.start;
        return line.start + math.projection(linetocenter, linevector);
    }
    Contact wheelcollider(RigidBody &body, LineSegment &line)
    {
        const float radius = body.m_radius;
        const sf::Vector2f center = m_dunebuggy.getTransform() * body.getPosition();

        const sf::Vector2f D = closetPointonSegment(center, line); // wrong, create virtual contact point when projection t > 1 and t < 0// solved
        const sf::Vector2f DA = center - D;
        float dist = math.getLength(DA);

        Contact contactinfo;
        if (dist > radius)
        {
            contactinfo.contact = {0, 0};
            contactinfo.normal = {0, 0};
            contactinfo.penetration_depth = 0;
            return contactinfo;
        }

        std::cout << '\n';
        contactinfo.contact = D;
        contactinfo.normal = math.getNormalized(DA);
        contactinfo.penetration_depth = radius - dist;
        // get the contact point with soon time of colision (maximum contact point is 2)

        return contactinfo;
    }
    bool checkcurrentframe(sf::Vector2f &localview)
    {
        std::pair<int, int> pair4 = findcarcollsion(localview);
        LineSegment line(m_stage[pair4.first], m_stage[pair4.second]);

        sf::FloatRect nullrect(0, 0, 0, 0);

        if (m_dunebuggy.getGlobalBounds().intersects(line.getGlobalBounds()) != nullrect)
            return true;
        // avoid tunneling when car drop out of road, this approach only work when the line is static.
        return false;
    }
    bool sweptnextframe(sf::Vector2f &localview)
    {
        // using swept aabb method
        sf::FloatRect sweptaabb = m_dunebuggy.getSweptGlobalBounds();
        std::pair<int, int> pairpos = findrangecollsion(sweptaabb, localview);

        LineSegment line(m_stage[pairpos.first], m_stage[pairpos.second]);

        // thinking of need using ray cast

        sf::FloatRect nullrect(0, 0, 0, 0);
        if (m_dunebuggy.getGlobalBounds().intersects(line.getGlobalBounds()) != nullrect)
            return true;
        return false;
    }
    bool BoardPhase(sf::RenderTarget &window, const sf::View &view)
    {
        // thinking of using next frame and current frame to avoid tunneling
        // instead using previous frame and current frame
        //
        // case 1:
        // update()
        // checkcollsion() -> using next - current frame -> return time of collision in range [0, 1];
        // and then multiply the updated moveamt by TOC // good
        // need a pretransform : prepos, prevel, preaccel, pre
        //
        // case 2:
        // checkcollsion() -> using previous - current frame -> return time of collision in range [0, 1];
        // and then solve penetration // bad
        // update()

        sf::Vector2f localview = window.mapPixelToCoords(sf::Vector2i(0, 0), view);

        // if current frame is not colliding and swept is true then mark it as precollision
        bool currentframe = checkcurrentframe(localview);
        bool nextframe = sweptnextframe(localview);
        std::pair<int, int> pair1;
        std::pair<int, int> pair2;
        std::pair<int, int> pair3;

        if (currentframe)
        {
            m_dunebuggy.collide = true;
            m_dunebuggy.precollide = false;
            pair1 = findrangecollsion(m_dunebuggy.m_wheels[0], localview);
            pair2 = findrangecollsion(m_dunebuggy.m_wheels[1], localview);
            pair3 = findrangecollsion(m_dunebuggy.m_carbody, localview);
        }
        if (!currentframe && sweptnextframe(localview))
        {
            m_dunebuggy.collide = false;
            m_dunebuggy.precollide = true;
            sf::FloatRect sweptaabb = m_dunebuggy.getSweptGlobalBounds();
            std::pair<int, int> pairpos = findrangecollsion(sweptaabb, localview);
            pair1 = pairpos;
            pair2 = pairpos;
            pair3 = pairpos;
        }

        manifold.pair1 = pair1;
        manifold.pair2 = pair2;
        manifold.pair3 = pair3;
        return currentframe || nextframe;
    }
    void predictCollisionTime(const sf::Vector2f &initpos, const sf::Vector2f &finalpos,
                              LineSegment &line, RigidBody &body)
    {
        if (body.shape != RigidBody::Shape::circle)
            return;
        const sf::Vector2f displacement = finalpos - initpos;
        if (math.getLength(displacement) == 0)
            return;
        const sf::Vector2f closet = ProjectVecOnLine(initpos, line);
        double distance = math.distance(closet, initpos);
        float move = distance - body.m_radius;

        sf::Vector2f inittocloset = closet - initpos;
        float cosine = math.getcosinevalue(displacement, inittocloset);
        float len = move / cosine;
        float tCollision = len / math.getLength(displacement);
        if (tCollision > 1)
            return; // no collision in next frame, dont need to check anymore;
    }
    void Generatecontact()
    {
        // method : find the minimum tcollision to avoid tunneling;
        manifold.Rearcontact.clear();
        manifold.Frontcontact.clear();
        manifold.Frontcontactcount = 0;
        manifold.Rearcontactcount = 0;
        for (int i = manifold.pair1.first; i < manifold.pair1.second; i += 2)
        {
            LineSegment line(m_stage[i], m_stage[i + 2]);
            Contact contact = wheelcollider(m_dunebuggy.m_wheels[0], line);
            if (contact.contact != sf::Vector2f(0, 0))
            {
                manifold.Rearcontact.push_back(contact);
                manifold.Rearcontactcount++;
            }
        }
        for (int i = manifold.pair2.first; i < manifold.pair2.second; i += 2)
        {
            LineSegment line(m_stage[i], m_stage[i + 2]);
            Contact contact = wheelcollider(m_dunebuggy.m_wheels[1], line);
            if (contact.contact != sf::Vector2f(0, 0))
            {
                manifold.Frontcontact.push_back(contact);
                manifold.Frontcontactcount++;
            }
        }
        // implement obb - linesegment intersection (just rotated back and perform collision check)
        // need code to obb;
        for (int i = manifold.pair3.first; i < manifold.pair3.second; i += 2)
        {
            LineSegment line(m_stage[i], m_stage[i + 2]);
        }
    }
    void NarrowPhase()
    {
        // narrow phase
        if (!boardphase)
            return;
        if (m_dunebuggy.collide)
        {
            Generatecontact();
            std::cout << '\n';
            printv(m_stage[87]);
            printv(m_stage[88]);
            printv(m_stage[89]);
            if (manifold.Rearcontactcount == 0 && manifold.Frontcontactcount == 0)
            {
                m_dunebuggy.collide = false;
                return;
            }
            for (int i = 0; i < manifold.Rearcontactcount; i++)
            {
                m_dunebuggy.move((manifold.Rearcontact[i].normal * manifold.Rearcontact[i].penetration_depth) / (float)manifold.Rearcontactcount);
            }
            for (int i = 0; i < manifold.Frontcontactcount; i++)
            {
                m_dunebuggy.move((manifold.Frontcontact[i].normal * manifold.Frontcontact[i].penetration_depth) / (float)manifold.Frontcontactcount);
            }

            Generatecontact(); // regenerate contact to minimum error;
        }
        if (m_dunebuggy.precollide)
        {
        }
        // reduces contact point to minimum value (like 2 or 1) or instead create a fake contact;
    }
    void solvewheels(RigidBody &body, RigidBody &ano)
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
    // update every thing
    void update(sf::RenderTarget &window, const sf::View &view, float dt)
    {
        m_dunebuggy.update(dt);
        boardphase = BoardPhase(window, view);
        NarrowPhase();

        m_dunebuggy.updateposition(dt);
    }
    void checkcollision()
    {
    }
    void syncview(const sf::View &view)
    {
        m_stage.syncWithView(view);
    }
    void render(sf::RenderWindow &window)
    {
        m_stage.draw(window);
        m_dunebuggy.draw(window);
    }
    ~World();

private:
    Hi hi;
    sm::sk math;
    Map m_stage;
    Car m_dunebuggy;
    sf::Texture tbody, twheel;
    float wheelradius = 30;

    CarManifold manifold;

    float m_substep;

    bool boardphase = false;
};
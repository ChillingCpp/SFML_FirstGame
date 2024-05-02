#pragma once
#include "Menu.hpp"
#include <functional>
#include <VertexDeque.hpp>
#include <iostream>
#include <PerlinNoise.hpp>
#define MAP_ADD_SIZE 200.f
class Map
{
public:
    Map() = default;
    Map(int size, float step) : size(size), step(step), road(sf::TriangleStrip, 1), dirt(sf::TriangleStrip, 1)
    {

        hi.shuffles(std::mt19937(905));
        initTexture();
        for (int i = 0 - MAP_ADD_SIZE / step; i < this->size + MAP_ADD_SIZE / step; i++)
        {
            float x = i * this->step;
            float y = yGen(x);
            float newY = y + roadthick;
            push_back({x, newY}, {x, y});
        }
    }
    void initTexture()
    {
        if (!m_troad.loadFromFile("D:/sfmlcpp/res/Game/surface.png"))
        {
            std::cout << "ERROR\n";
        }
        if (!m_tdirt.loadFromFile("D:/sfmlcpp/res/Game/ground.png"))
        {
        }
        m_troad.setSmooth(true);
        m_tdirt.setSmooth(true);
    }
    void initMap(float step, int size)
    {
        this->step = step;
        this->size = size;
        hi.shuffles(std::mt19937(905));
        road.initall(sf::TriangleStrip, 1);
        dirt.initall(sf::TriangleStrip, 1);
        initTexture();
        for (int i = 0 - MAP_ADD_SIZE / step; i < this->size + MAP_ADD_SIZE / step; i++)
        {
            float x = i * this->step;
            float y = yGen(x);
            float newY = y + roadthick;
            push_back({x, newY}, {x, y});
        }
    }
    ~Map();
    sf::Vector2f &operator[](int index)
    {
        return road[index - indexoffset + 1].position;
    }

    const sf::Vector2f &operator[](int index) const
    {
        return road[index - indexoffset + 1].position;
    }
    float getStep()
    {
        return step;
    }
    int getSize()
    {
        return road.size();
    }
    int getwindowsize()
    {
        return size;
    }
    void push_front(sf::Vector2f newcoord, sf::Vector2f coord)
    {

        road.prepend(sf::Vertex(coord, sf::Vector2f(std::fmod(coord.x, 1024.f), 23))); // 23 and 48 is constant pixel
        road.prepend(sf::Vertex(newcoord, sf::Vector2f(std::fmod(newcoord.x, 1024.f), 48)));

        dirt.prepend(sf::Vertex(sf::Vector2f(coord.x, coord.y + roadthick), sf::Vector2f(std::fmod(coord.x, 1024.f), 1)));
        dirt.prepend(sf::Vertex(sf::Vector2f(newcoord.x, 1000000), sf::Vector2f(std::fmod(newcoord.x, 1024.f), 1023)));
    }
    void push_back(sf::Vector2f newcoord, sf::Vector2f coord)
    {

        road.append(sf::Vertex(coord, sf::Vector2f(std::fmod(coord.x, 1024.f), 23))); // 23 and 48 is constant pixel
        road.append(sf::Vertex(newcoord, sf::Vector2f(std::fmod(newcoord.x, 1024.f), 48)));

        dirt.append(sf::Vertex(sf::Vector2f(coord.x, coord.y + roadthick), sf::Vector2f(std::fmod(coord.x, 1024.f), 1)));
        dirt.append(sf::Vertex(sf::Vector2f(newcoord.x, 1000000), sf::Vector2f(std::fmod(newcoord.x, 1024.f), 1023)));
    }
    void pop_back()
    {
        road.pop_back();
        road.pop_back();
        dirt.pop_back();
        dirt.pop_back();
    }
    void pop_front()
    {
        road.pop_front();
        road.pop_front();
        dirt.pop_front();
        dirt.pop_front();
    }
    void syncWithView(const sf::View &view)
    {
        float left = view.getCenter().x - view.getSize().x / 2 - MAP_ADD_SIZE;
        float right = view.getCenter().x + view.getSize().x / 2 + MAP_ADD_SIZE;
        // Append more points
        while (road[road.size() - 1].position.x < right)
        {
            float x = road[road.size() - 1].position.x + step;
            float y = yGen(x);

            float newX = road[road.size() - 1].position.x;
            float newY = yGen(newX) + roadthick;
            sf::Vector2f coord(x, y);
            sf::Vector2f newcoord(newX, newY);

            push_back(newcoord, coord);
            // doesn't affect indexoffset
        }
        // Prepend more points
        while (road[2].position.x > left)
        {
            float newX = road[0].position.x - step;
            float newY = yGen(newX);
            float x = road[0].position.x - step;
            float y = yGen(x) + roadthick;

            sf::Vector2f coord(newX, newY);
            sf::Vector2f befcoord(x, y);
            push_front(befcoord, coord);
            indexoffset -= 2;
        }
        // Trim right points
        while (road[road.size() - 1].position.x > right)
        {
            pop_back();
            // doesn't affect indexoffset
        }
        // Trim left points
        while (road[2].position.x < left)
        {
            pop_front();
            indexoffset += 2;
        }
    }
    float yGen(float &x)
    {
        const float xScale = 0.0005f;
        const float yMagnitude1 = 560;
        const float yMagnitude2 = 720;
        const float yMagnitude3 = 500;
        const float yMagnitude4 = 50;

        const float xScaled = x * xScale;
        return float(yMagnitude1 * hi.noise(xScaled * 0.5) +
                     yMagnitude2 * hi.noise(xScaled * 1) +
                     yMagnitude3 * hi.noise(xScaled * 2) +
                     yMagnitude4 * hi.noise(xScaled * 4) + height);
    };
    void draw(sf::RenderWindow &window)
    {
        window.draw(road, &m_troad);
        window.draw(dirt, &m_tdirt);
    }

private:
    VertexContiguousDeque road;
    VertexContiguousDeque dirt;

    sf::Texture m_troad, m_tdirt;

    int indexoffset = 0;
    float roadthick = 10;
    float step;
    int size;
    float height = 500.f;
    Hi hi;
};

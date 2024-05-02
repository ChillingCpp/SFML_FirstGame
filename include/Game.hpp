#pragma once

#include <World.hpp>
class Game
{
public:
    void initCar(sf::RenderWindow &W);
    void init(sf::RenderWindow &W);
    void initTexture();
    void Event(sf::Event &e, sf::RenderWindow &w, sf::Sound &sound, sf::View &view, bool hassound);
    void playSound(sf::Sound &sound, bool hassound);
    void stopSound(sf::Sound &sound, bool hassound);
    void updateSound(sf::Sound &sound, bool hassound);

    void pause(sf::RenderWindow &w, sf::View &view);
    void setPause(bool pauseopt);
    void draw(sf::RenderWindow &w, sf::View &view,  sf::Sound &sound, bool hassound);

    bool backtolobby();
    bool findGlobal(sf::RenderWindow &w, sf::Sprite &button, sf::View &view);
    int findGlobal(sf::RenderWindow &w, std::vector<sf::Sprite> &button, sf::View &view);

    // generate map
    Game(sf::RenderWindow &w, float step, int size)
    {
        winX = w.getSize().x;
        winY = w.getSize().y;
        initTexture();
        initCar(w);
        init(w);
        m_world.initall(step, size);
        
    }
    void push_back(sf::Vector2f newcoord, sf::Vector2f coord);
    void push_front(sf::Vector2f newcoord, sf::Vector2f coord);
    void pop_back();
    void movebg(float movement);
    void pop_front();
    void syncWithView(const sf::View &view);
    void update(sf::RenderWindow &w, sf::View &view, float dt);

    ~Game();

private:
    sf::Texture pausetexture;
    sf::Sprite pausebutton;

    sf::Texture pausepopup;
    sf::Sprite pausescreen;
    sf::RectangleShape blurbg;

    std::vector<sf::Sprite> pauseoption;
    std::vector<sf::Texture> pauseT;

    sf::Texture tbg;
    std::vector<sf::Vertex> background;

    sf::FloatRect pauseclose;

    sf::Font font;
    sf::Text t;

    World m_world;

    float winX, winY;
    float separate;
    float buttonX = 300, butonY = 90;
    float XPause = 682, YPause = 59;
    float tbgX, tbgY;
    float scale = 99, height = 10;

    int meter = 0;
    int clickE = 0;

    bool isPause = false, isGame = true, isGamerun = true, genmap = false;

    std::string direction;
};
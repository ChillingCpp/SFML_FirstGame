#pragma once

#include <Menu.hpp>
class Option
{
public:
    Option(sf::RenderWindow &w, sf::Music& music, sf::Sound& sound);
    void init(sf::RenderWindow &w, sf::Music& music, sf::Sound& sound);
    void Event(sf::Event &e, sf::RenderWindow &w, sf::Sound& sound);
    void draw(sf::RenderWindow& w);

    void stopMusic(sf::Music& music);
    void playMusic(sf::Music& music);
    void playSound(sf::Sound& sound);
    void stopSound(sf::Sound& sound);
    void update(sf::Music& music);
    void updateSound(sf::Sound& sound);
    
    bool getinit();
    bool getSoundStatus();
    bool backtomenu();

    ~Option();

private:
    
    sf::RectangleShape back;
    sf::RectangleShape Bsound;
    sf::RectangleShape Bmusic;
    sf::RectangleShape tickS;
    sf::RectangleShape tickM;
    
    sf::Text title;
    sf::Text tmusic;
    sf::Text tsound;

    sf::Texture texture;
    sf::Texture tick;

    sf::Font font;

    sf::Music music;

    float backsizeX = 150, backsizeY = 100;
    float addsize = 20;
    float winX, winY;

    bool hasinit = false;
    bool hassound = true;
    bool hasmusic = true;
    int clickB =0;
};

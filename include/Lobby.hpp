
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

class Lobby
{
public:

    void init(sf::RenderWindow &w);
    void initTexture();
    void Event(sf::Event &e, sf::RenderWindow& w, sf::Sound &sound, bool hassound);
    void backtomenu(sf::RenderWindow& w);
    void setDefault();

    bool getDefaultconfig();
    bool option();
    bool startGame();

    void rePosition(sf::RenderWindow &w);
    void playSound(sf::Sound& sound, bool hassound);
    void stopSound(sf::Sound& sound, bool hassound);
    void updateSound(sf::Sound& sound, bool hassound);
    void draw(sf::RenderWindow &w); 
    
    void stage();
    void vehicle();
    void tune();

    void upgradetune(int tunepos);

    void resetOption();
    

    int Binsearch(std::vector<sf::Sprite> &button);
    int findGlobal(sf::WindowBase &w, std::vector<sf::Sprite> &button);
    bool findGlobal(sf::WindowBase &w,  sf::Sprite &button);

    Lobby(sf::RenderWindow& w);
    ~Lobby();

private:

    std::vector<sf::Sprite> button;

    std::vector<sf::Texture> t;

    sf::Texture tmap;
    sf::RectangleShape Smap;
    std::vector<sf::IntRect> Rmap;

    sf::Texture tvehicle;
    sf::RectangleShape Svehicle;
    std::vector<sf::IntRect> Rvehicle;

    std::vector<sf::Texture> ttune;
    std::vector<sf::Sprite> Stune;

    sf::Font fontLobby;

    sf::Text textStage;
    sf::Text textVehicle;
    std::vector<sf::Text> upgradeCost;

    sf::RectangleShape setting;
    sf::Texture tsetting;

    sf::RectangleShape blurbg;
    sf::Texture popup;
    sf::Sprite Spopup;

    float buttonX = 125, buttonY = 75;
    float addsize = 15;
    float separate = 37.5;
    float winX, winY;
    float buttontoborderX = 50, buttontoborderY = 150;

    float separateT = 20;
    float separateTborderX = 32.5;
    
    int bnum = 5;
    int tunelevel = 0;
    int pricecost = 0;
    int clickB = 0;
    int clickS = 0;

    bool isStage = true, isVehicle = false, isTune = false;
    bool isOption = false;
    bool isPopup = false, iscap = false, isdefault = false;

};

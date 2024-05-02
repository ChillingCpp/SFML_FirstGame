#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Option.hpp>
#include <Lobby.hpp>
#include <Game.hpp>
int main()
{
    sf::RenderWindow w(sf::VideoMode(1000, 708), "Hi");
    w.setPosition(sf::Vector2i(179, 0));

    int frame_rate = 60;
    w.setFramerateLimit(frame_rate);
    
    float winWidth = w.getSize().x;
    float winHeight = w.getSize().y;
    
    // not want to be in any class

    sf::View view = w.getView();
    view.setCenter(winWidth /2, winHeight /2);

    sf::Music music;
    if (!music.openFromFile("D:/sfmlcpp/res/menu/Sweden-mc-music.ogg")){
        //
    }
    sf::SoundBuffer soundbuffer;
    if (!soundbuffer.loadFromFile("D:/sfmlcpp/res/menu/click.ogg")){
        //
    }

    sf::Sound sound;
    sound.setBuffer(soundbuffer);
    
    sf::Texture bgtext;
    bgtext.loadFromFile("D:/sfmlcpp/res/menu/menu-bg.png");

    std::vector<sf::Vertex> bg;

    float bgX = bgtext.getSize().x;
    float bgY = bgtext.getSize().y;
    
    bg.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color(255,255,255), sf::Vector2f(0, 0)));
    bg.push_back(sf::Vertex(sf::Vector2f(w.getSize().x, 0), sf::Color(255,255,255), sf::Vector2f(bgX, 0)));
    bg.push_back(sf::Vertex(sf::Vector2f(w.getSize()), sf::Color(255,255,255), sf::Vector2f(bgX, bgY)));
    bg.push_back(sf::Vertex(sf::Vector2f(0, w.getSize().y), sf::Color(255,255,255), sf::Vector2f(0, bgY)));

    // Option
    Option option(w, music, sound);
    Lobby lobby(w);

    bool isStart = false, isLobby = true, isOption = false, exit = false;
    bool optioninlobby = false, stopgame = false;
    bool hassound = 1, hasmusic = 1;

    sf::Clock clock;
    float deltatime = 1.f/frame_rate;
    float fps = 0;
    sf::Text Fps;
    sf::Font font;
    font.loadFromFile("D:/sfmlcpp/res/mono.ttf");
    Fps.setFont(font);
    Fps.setCharacterSize(30);
    Fps.setFillColor(sf::Color(255, 0, 0));

    float step = 20.f;
    int size = std::ceil(w.getSize().x / step)+1;
    
    Game game(w, step, size);
    

    // idea for faster game : uses std::vector to store option, lobby, menu class
    // when isOption or isMenu or isLobby = false then pop back that class from vector 
    // failed because music cant be copied

    while (w.isOpen()){
        float time = clock.restart().asSeconds();
        fps = 1 / time;

        Fps.setString(std::to_string(fps));
        Fps.setOrigin(Fps.getLocalBounds().width, Fps.getLocalBounds().height);
        Fps.setPosition(winWidth - 10 + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, winHeight - Fps.getOrigin().y);

        sf::Event e;
        while(w.pollEvent(e)){
            switch (e.type)
            {
            case sf::Event::Closed:
                w.close();
                break;
            case sf::Event::GainedFocus:
                std::cout << "focus\n";
                break;
            case sf::Event::LostFocus:
                std::cout << "lost\n";
                // stopgame = true;
                break;
            case sf::Event::KeyPressed:
                if (e.key.scancode == sf::Keyboard::Scan::Escape){
                    w.close();
                }
            default:
                break;
            }
        
            if (isOption) option.Event(e, w, sound);
            if (isLobby) lobby.Event(e, w, sound, option.getSoundStatus());
            if (isStart) game.Event(e, w, sound, view, option.getSoundStatus());

        }
        w.clear();
        w.setView(view); 
        if (!isStart) w.draw(&bg[0], bg.size(), sf::Quads, &bgtext);
        if (isLobby){
            view.setCenter(winWidth/2, winHeight/2);
            if (!lobby.getDefaultconfig()) lobby.setDefault();
            if (!optioninlobby) lobby.draw(w);
            if (lobby.option()){
                isOption = true;
                optioninlobby = true;
            }
            if (lobby.startGame()){
                isLobby = false;
                isStart = true;
            }
        }
        if (isOption){
            
            option.draw(w);
            option.update(music);
            if (option.backtomenu()) {
                if (isLobby){
                    isLobby = false;
                    isLobby = true;
                }
                optioninlobby = false;
                lobby.resetOption();
                isStart = false;
                isOption = false;
            }
        }
        if (isStart) {
            // game.setPause(stopgame);
            game.update(w, view ,time);
            game.draw(w, view, sound, option.getSoundStatus());
            
            if (game.backtolobby()){
                exit = true;
                isStart = false;
                isOption = false;
                isLobby = true;
            }
        }
        w.draw(Fps);
        w.display();
    }
    isLobby = false;
    return 0;
}
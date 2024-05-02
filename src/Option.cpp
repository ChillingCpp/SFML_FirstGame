#include <Option.hpp>

void Option::init(sf::RenderWindow &w, sf::Music& music, sf::Sound& sound){

    if (!texture.loadFromFile("D:/sfmlcpp/res/menu/back.png")){
        //
    }
    if (!font.loadFromFile("D:/sfmlcpp/res/mono.ttf")){
        //
    }
    if (!tick.loadFromFile("D:/sfmlcpp/res/menu/tick.png")){
        //
    }
    texture.setSmooth(true);
    tick.setSmooth(true);

    winX = w.getSize().x;
    winY = w.getSize().y;

    back.setSize(sf::Vector2f(backsizeX, backsizeY));
    back.setOrigin(backsizeX / 2, backsizeY /2);
    back.setTexture(&texture);
    back.setPosition(50+back.getOrigin().x, winY-50-back.getOrigin().y);

    Bsound.setSize(sf::Vector2f(100, 100));
    Bsound.setOrigin(Bsound.getSize().x /2, Bsound.getSize().y /2);
    Bsound.setPosition(winX/2 + 150 + back.getOrigin().x, winY /2 - 2.2*Bsound.getSize().y);
    Bsound.setFillColor(sf::Color(255, 255, 255));

    Bmusic.setSize(sf::Vector2f(100, 100));
    Bmusic.setOrigin(Bmusic.getSize().x /2, Bmusic.getSize().y /2);
    Bmusic.setPosition(winX/2 + 150 + back.getOrigin().x, winY /2 -0.8* Bmusic.getSize().y);
    Bmusic.setFillColor(sf::Color(255, 255, 255));

    tickS.setSize(sf::Vector2f(100, 100));
    tickS.setOrigin(tickS.getSize().x /2, tickS.getSize().y /2);
    tickS.setTexture(&tick);
    tickS.setPosition(winX/2 + 150 + back.getOrigin().x, winY /2 - 2.2*Bsound.getSize().y);

    tickM.setSize(sf::Vector2f(100, 100));
    tickM.setOrigin(tickM.getSize().x /2, tickM.getSize().y /2);
    tickM.setTexture(&tick);
    tickM.setPosition(winX/2 + 150 + back.getOrigin().x, winY /2 -0.8* Bmusic.getSize().y);

    title = sf::Text("Options", font, 50);
    title.setOrigin(title.getLocalBounds().width/2, title.getLocalBounds().height/2);
    title.setPosition(winX/2, title.getOrigin().y*3);

    tsound = sf::Text("sound", font, 50);
    tsound.setOrigin(tsound.getLocalBounds().width/2, tsound.getLocalBounds().height/2);
    tsound.setPosition(winX/2 - 300, winY /2 - 2.2*Bsound.getSize().y);

    tmusic = sf::Text("music", font, 50);
    tmusic.setOrigin(tmusic.getLocalBounds().width/2, tmusic.getLocalBounds().height/2);
    tmusic.setPosition(winX/2 - 300, winY /2 - 0.8*Bmusic.getSize().y);

    update(music);
    hasinit = true;
}
void Option::Event(sf::Event& e, sf::RenderWindow &w, sf::Sound& sound){
    
    if (back.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
        back.setFillColor(sf::Color(12, 170, 255));
    }
    else back.setFillColor(sf::Color(255, 255, 255));
    if (e.type == sf::Event::MouseButtonPressed){
        if (e.mouseButton.button == sf::Mouse::Left){

            if (back.getSize().x > backsizeX && back.getSize().y > backsizeY){
                back.setSize(sf::Vector2f(backsizeX, backsizeY));
                back.setOrigin(backsizeX / 2, backsizeY /2);
                back.setPosition(50+back.getOrigin().x, winY-50-back.getOrigin().y);                    
            }
            if (back.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
                back.setSize(sf::Vector2f(backsizeX + addsize, backsizeY + addsize));
                back.setOrigin(back.getSize().x / 2, back.getSize().y /2);
                clickB++;
                updateSound(sound);
            }
            else if (Bsound.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
                if (hassound) {hassound = false; updateSound(sound); tickS.setPosition(999999, 999999);}
                else {hassound = true; updateSound(sound); tickS.setPosition(Bsound.getPosition());}
            }
            else if (Bmusic.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
                if (hasmusic) {hasmusic = false; updateSound(sound); tickM.setPosition(9999999, 9999999);}
                else {hasmusic = true; updateSound(sound); tickM.setPosition(Bmusic.getPosition());}
            }
            else clickB = 0;
        }
    }
}
bool Option::getinit(){
    if (hasinit) return true;
    return false;
}
bool Option::backtomenu(){
    if (clickB == 2){ 
        back.setSize(sf::Vector2f(backsizeX, backsizeY));
        back.setOrigin(backsizeX / 2, backsizeY /2);
        back.setPosition(50+back.getOrigin().x, winY-50-back.getOrigin().y);
        clickB = 0; 
        return true;
    }
    return false;
}
void Option::draw(sf::RenderWindow &w){

    w.draw(back);
    w.draw(Bsound);
    w.draw(Bmusic);

    w.draw(title);
    w.draw(tmusic);
    w.draw(tsound);

    w.draw(tickS);
    w.draw(tickM);
}
void Option::playMusic(sf::Music& music){
    if (hasmusic && music.getStatus() != sf::SoundSource::Playing) {music.play(); music.setLoop(true);}
}
void Option::stopMusic(sf::Music& music){
    if (hasmusic == false && music.getStatus() == sf::SoundSource::Playing) music.stop();
}
void Option::playSound(sf::Sound& sound){
    if (hassound) sound.play();
}
void Option::stopSound(sf::Sound& sound){
    if (hassound == false && sound.getStatus() == sf::SoundSource::Playing) sound.stop();
}
void Option::update(sf::Music& music){
    playMusic(music);
    stopMusic(music);
}
void Option::updateSound(sf::Sound& sound){
    playSound(sound);
    stopSound(sound);
}
bool Option::getSoundStatus(){
    return hassound;
}
Option::Option(sf::RenderWindow &w, sf::Music& music, sf::Sound& sound)
{
    init(w, music, sound);
}

Option::~Option()
{

}
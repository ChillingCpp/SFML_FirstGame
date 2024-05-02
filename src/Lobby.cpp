#include <Lobby.hpp>
void Lobby::initTexture(){
    for (int i = 0; i < bnum; i++)
        t.push_back(sf::Texture());
    
    for (int i = 0; i < 4; i++)
        ttune.push_back(sf::Texture());

    if (!t[0].loadFromFile("D:/sfmlcpp/res/menu/goback.png")){}

    if (!t[1].loadFromFile("D:/sfmlcpp/res/menu/stage.png")){}

    if (!t[2].loadFromFile("D:/sfmlcpp/res/menu/vehicle.png")){}

    if (!t[3].loadFromFile("D:/sfmlcpp/res/menu/tune.png")){}

    if (!t[4].loadFromFile("D:/sfmlcpp/res/menu/start.png")){}


    if (!tmap.loadFromFile("D:/sfmlcpp/res/menu/stage-icons-0.jpg")){}

    if (!tvehicle.loadFromFile("D:/sfmlcpp/res/menu/car-icons-1.jpg")){}

    if (!ttune[0].loadFromFile("D:/sfmlcpp/res/menu/Picsart/MMengine.png")){}
    if (!ttune[1].loadFromFile("D:/sfmlcpp/res/menu/Picsart/MMsuspension.png")){}
    if (!ttune[2].loadFromFile("D:/sfmlcpp/res/menu/Picsart/MMtires.png")){}
    if (!ttune[3].loadFromFile("D:/sfmlcpp/res/menu/Picsart/MMfuel.png")){}
    
    if (!fontLobby.loadFromFile("D:/sfmlcpp/res/ChangaOne-Regular.ttf")){}
    
    if (!tsetting.loadFromFile("D:/sfmlcpp/res/menu/button-settings.png")){}
    if (!popup.loadFromFile("D:/sfmlcpp/res/menu/popup-small.png")){}
    
    t[0].setSmooth(true);
    t[1].setSmooth(true);
    t[2].setSmooth(true);
    t[3].setSmooth(true);
    t[4].setSmooth(true);

    tmap.setSmooth(true);
    tvehicle.setSmooth(true);
    for (int i = 0; i < 4; i++){
        ttune[i].setSmooth(true);
    }
    tsetting.setSmooth(true);
    popup.setSmooth(true);
}
void Lobby::setDefault(){
    if (!isdefault){
        isStage = true;
        button[1].setScale((buttonX + addsize) / buttonX, (buttonY + addsize) / buttonY);
        button[1].setOrigin(
        button[1].getGlobalBounds().width / 2, button[1].getGlobalBounds().height / 2);
        button[1].setPosition(buttonX + buttontoborderX + addsize /2  + 1* (buttonX + separate), winY - -addsize /2 - buttontoborderY);
        button[1].setColor(sf::Color(57, 231, 95));
        button[4].setScale(1, 1);
        button[4].setOrigin(buttonX /2, buttonY/2);
        button[4].setPosition(buttonX + buttontoborderX + 4* (buttonX + separate), winY - buttontoborderY);
        isOption = false;
        isVehicle = false;
        isTune = false;
        isdefault = true;
    }
}
void Lobby::init(sf::RenderWindow &w){
    winX = w.getSize().x;
    winY = w.getSize().y;

    Rmap.push_back(sf::IntRect(1, 775, 512, 256));
    Rvehicle.push_back(sf::IntRect(1543, 1, 256,512));

    for (int i = 0;i < 5; i++){
        button.push_back(sf::Sprite(t[i]));
    }
    for (int i = 0; i < button.size() ;i++){
        button[i].setOrigin(t[i].getSize().x /2, t[i].getSize().y/2);
        button[i].setPosition(buttonX + buttontoborderX + i* (buttonX + separate), winY - buttontoborderY);
    }
   
    Smap.setSize(sf::Vector2f(600, 300));
    Smap.setOrigin(Smap.getSize().x /2, Smap.getSize().y /2);
    Smap.setPosition(winX/2, winY/2- buttontoborderY /2);
    Smap.setTexture(&tmap);
    Smap.setTextureRect(Rmap[0]);

    Svehicle.setSize(sf::Vector2f(300, 600));
    Svehicle.setOrigin(Svehicle.getSize().x /2, Svehicle.getSize().y /2);
    Svehicle.setPosition(winX/2, winY/2- buttontoborderY /2);
    Svehicle.setTexture(&tvehicle);
    Svehicle.setTextureRect(Rvehicle[0]);
    Svehicle.rotate(-90);
    
    for (int i = 0; i< 4; i++){
        Stune.push_back(sf::Sprite(ttune[i]));
        upgradeCost.push_back(sf::Text(std::to_string(pricecost) + "\n" + std::to_string(tunelevel) + "/20", fontLobby, 35));
    }
    for (int i = 0; i < 4; i++){
        Stune[i].setOrigin(Stune[i].getGlobalBounds().width /2, Stune[i].getGlobalBounds().height /2);
        Stune[i].setPosition(ttune[i].getSize().x + separateTborderX + i* (ttune[i].getSize().x + separateT), 
        winY/2 - buttontoborderY/1.9);
    }
    for (int i = 0; i < 4; i++){
        upgradeCost[i].setPosition(Stune[i].getPosition().x, 
        Stune[i].getPosition().y - Stune[i].getLocalBounds().width/2 - 35/1.5);
    }
    textStage = sf::Text("COUNTRYSIDE", fontLobby, 50);
    textStage.setOrigin(textStage.getLocalBounds().width/2,
    textStage.getLocalBounds().height/2);
    textStage.setPosition(Smap.getPosition().x, Smap.getPosition().y/2 - textStage.getLocalBounds().height - 10);

    textVehicle = sf::Text("DUNE BUGGY", fontLobby, 50);
    textVehicle.setOrigin(textVehicle.getLocalBounds().width/2,
    textVehicle.getLocalBounds().height/2);
    textVehicle.setPosition(Svehicle.getPosition().x, Svehicle.getPosition().y/2 - textVehicle.getLocalBounds().height- 10);

    setting.setSize(sf::Vector2f(100, 100));
    setting.setOrigin(setting.getSize().x /2, setting.getSize().y /2);
    setting.setPosition(winX - setting.getOrigin().x, setting.getOrigin().y);
    setting.setTexture(&tsetting);

    blurbg.setSize(sf::Vector2f(w.getSize()));
    blurbg.setOrigin(w.getSize().x /2, w.getSize().y/2);
    blurbg.setFillColor(sf::Color(0,0,0, 128));
    blurbg.setPosition(blurbg.getOrigin());

    Spopup.setTexture(popup);
    Spopup.setOrigin(popup.getSize().x /2, popup.getSize().y /2);
    Spopup.setPosition(w.getSize().x/2, w.getSize().y/2);

    setDefault();
    
}
int Lobby::Binsearch(std::vector<sf::Sprite> &button){

    int i = 0;
    for (i = 0; i < button.size(); i++){
        if (button[i].getGlobalBounds().width > buttonX && button[i].getGlobalBounds().height > buttonY){
            return i;
        }
    }
    return -1;
}
int Lobby::findGlobal(sf::WindowBase &w, std::vector<sf::Sprite> &button){

    int j = 0;
    for (j = 0; j < button.size(); j++){
        
        if (button[j].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){;
            return j;
        }
    }
    return -1;
}
bool Lobby::findGlobal(sf::WindowBase &w,  sf::Sprite &button){

    if (button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w))))
   
    return true;
    
    return false;
}
void Lobby::Event(sf::Event& e, sf::RenderWindow& w, sf::Sound& sound, bool hassound){
    
    int k = Binsearch(button);
    for (int i = 0; i < button.size(); i++){
        
        if (button[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
            button[i].setColor(sf::Color(12, 170, 255));
        }
        else button[i].setColor(sf::Color(255, 255, 255));
    
        if (k != -1) button[k].setColor(sf::Color(57, 231, 95));
    }
    if (e.type == sf::Event::MouseButtonPressed){
        if (e.mouseButton.button == sf::Mouse::Left){   
            if (setting.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)))){
                isOption = true;
            }         
            int j = findGlobal(w, button);
            
            if (j != -1){
                int i = Binsearch(button);
                if (i != -1){
                    button[i].setScale(1, 1);
                    button[i].setOrigin(t[i].getSize().x /2, t[i].getSize().y/2);
                    button[i].setPosition(buttonX + buttontoborderX + i* (buttonX + separate), winY - buttontoborderY);
                }
                button[j].setScale((buttonX + addsize) / buttonX, (buttonY + addsize) / buttonY);
                button[j].setOrigin(
                button[j].getGlobalBounds().width / 2, button[j].getGlobalBounds().height / 2);
                button[j].setPosition(buttonX + buttontoborderX + addsize /2  + j* (buttonX + separate), winY - -addsize /2 - buttontoborderY);
                updateSound(sound, hassound);
                if (j == 0) clickB++;
                else clickB = 0;
                if (j == 4) clickS++;
                else clickS = 0;

            }

            if (isTune){
                
                int it = findGlobal(w, Stune);
                if (it != - 1){
                    updateSound(sound, hassound);
                    isPopup = true;
                }
                if (isPopup){
                
                    bool it2 = findGlobal(w, Spopup);
                    if (!it2 && it == -1){
                        updateSound(sound, hassound);
                        isPopup = false;
                    }
                }
                
            }
            
            if (j == 1) {isStage = true; stage();}
            if (j == 2) {isVehicle = true; vehicle();}
            if (j == 3) {isTune = true; tune();}
        }
    }
    if (clickB == 2) backtomenu(w);   
}
bool Lobby::getDefaultconfig(){
    return isdefault;
}
bool Lobby::option(){
    return isOption;
}
void Lobby::resetOption(){
    isOption = false;
}
void Lobby::backtomenu(sf::RenderWindow &w){
    if (clickB == 2)
    w.close();
}
bool Lobby::startGame(){
    if (clickS == 2){
        isStage = false;
        isOption = false;
        isVehicle = false;
        isTune = false;
        isdefault =false;
        clickS = 0;
        return true;
    }
    return false;
}
void Lobby::stage(){
    
    if (isStage){
        isVehicle = false;
        isTune = false;
        return;
    }
    return;
}
void Lobby::vehicle(){
    
    if (isVehicle){
        isStage = false;
        isTune = false;
        return;
    }
    return;
}
void Lobby::tune(){
    
    if (isTune){
        isStage = false;
        isVehicle = false;
    }
}
Lobby::Lobby(sf::RenderWindow &w)
{
    initTexture();
    init(w);
}

Lobby::~Lobby()
{

}
void Lobby::playSound(sf::Sound& sound, bool hassound){
    if (hassound) sound.play();
}
void Lobby::stopSound(sf::Sound& sound, bool hassound){
    if (hassound == false && sound.getStatus() == sf::SoundSource::Playing) sound.stop();
}
void Lobby::updateSound(sf::Sound& sound, bool hassound){
    playSound(sound, hassound);
    stopSound(sound, hassound);
}
void Lobby::draw(sf::RenderWindow &w){
    for (int i = 0; i < button.size(); i++){
        w.draw(button[i]);
    }
    if (isStage) {
        w.draw(Smap);
        w.draw(textStage);
    }
    if (isVehicle){

        w.draw(Svehicle);
        w.draw(textVehicle);
    } 
    if (isTune) {
        for (int i = 0; i < 4; i++){
            w.draw(Stune[i]);
            w.draw(upgradeCost[i]);
        }   
    }
    if (isPopup){
            w.draw(blurbg);
            w.draw(Spopup);
        }
    
    w.draw(setting);
}
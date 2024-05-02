#include <Game.hpp>

void Game::initTexture()
{

    for (int i = 0; i < 3; i++)
    {
        pauseT.push_back(sf::Texture());
    }

    if (!pauseT[0].loadFromFile("D:/sfmlcpp/res/Game/resume-button.png"))
    {
    }
    if (!pauseT[1].loadFromFile("D:/sfmlcpp/res/Game/restart-button.png"))
    {
    }
    if (!pauseT[2].loadFromFile("D:/sfmlcpp/res/Game/exit-button.png"))
    {
    }

    if (!pausepopup.loadFromFile("D:/sfmlcpp/res/Game/PauseScreen1.png"))
    {
    }
    if (!pausetexture.loadFromFile("D:/sfmlcpp/res/Game/CloseNormal.png"))
    {
    }
    if (!tbg.loadFromFile("D:/sfmlcpp/res/Game/sky-bg_HD.png"))
    {
    }
    if (!font.loadFromFile("D:/sfmlcpp/res/ChangaOne-Regular.ttf"))
    {
    }


    pauseT[0].setSmooth(true);
    pauseT[1].setSmooth(true);
    pauseT[2].setSmooth(true);

    pausepopup.setSmooth(true);
    pausetexture.setSmooth(true);
    tbg.setSmooth(true);

}
void Game::initCar(sf::RenderWindow &w)
{

}
void Game::init(sf::RenderWindow &w)
{
    
    pauseclose = sf::FloatRect(XPause, YPause, 80, 80);

    for (int i = 0; i < 3; i++)
    {
        pauseoption.push_back(sf::Sprite(pauseT[i]));
    }

    tbgX = tbg.getSize().x;
    tbgY = tbg.getSize().y;

    background.push_back(sf::Vertex(sf::Vector2f(-500000, -500000), sf::Color(255, 255, 255), sf::Vector2f(0, 0)));
    background.push_back(sf::Vertex(sf::Vector2f(winX + 500000, -500000), sf::Color(255, 255, 255), sf::Vector2f(tbgX, 0)));
    background.push_back(sf::Vertex(sf::Vector2f(-500000, 500000), sf::Color(255, 255, 255), sf::Vector2f(tbgX, tbgY)));
    background.push_back(sf::Vertex(sf::Vector2f(500000, 500000), sf::Color(255, 255, 255), sf::Vector2f(0, tbgY)));

    pausescreen.setTexture(pausepopup);
    pausescreen.setOrigin(pausepopup.getSize().x / 2, pausepopup.getSize().y / 2);
    pausescreen.setPosition(winX / 2, winY / 2);

    for (int i = 0; i < 3; i++)
    {
        pauseoption[i].setOrigin(pauseT[i].getSize().x / 2, pauseT[i].getSize().y / 2);
        pauseoption[i].setPosition(winX / 2, 70 + 70 * i + (3 + (1.5 * i)) * (((winY - pausescreen.getOrigin().y * 2) / 2)));
    }

    pausebutton.setTexture(pausetexture);
    pausebutton.setOrigin(pausetexture.getSize().x / 2, pausetexture.getSize().x / 2);
    pausebutton.setPosition(winX - pausebutton.getOrigin().x, pausebutton.getOrigin().y);

    blurbg.setSize(sf::Vector2f(w.getSize()));
    blurbg.setOrigin(w.getSize().x / 2, w.getSize().y / 2);
    blurbg.setFillColor(sf::Color(0, 0, 0, 128));
    blurbg.setPosition(blurbg.getOrigin());

    t = sf::Text(std::to_string(meter) + 'm', font, 30);
    t.setOrigin(t.getLocalBounds().width / 2, t.getLocalBounds().height / 2);
    t.setPosition(t.getOrigin());

}

int Game::findGlobal(sf::RenderWindow &w, std::vector<sf::Sprite> &button, sf::View &view)
{

    for (int i = 0; i < button.size(); i++)
    {
        if (button[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)) + sf::Vector2f(w.mapPixelToCoords(sf::Vector2i(0, 0), view))))
        {
            return i;
        }
    }
    return -1;
}
bool Game::findGlobal(sf::RenderWindow &w, sf::Sprite &button, sf::View &view)
{

    if (button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w)) + sf::Vector2f(w.mapPixelToCoords(sf::Vector2i(0, 0), view))))

        return true;

    return false;
}
void Game::Event(sf::Event &e, sf::RenderWindow &w, sf::Sound &sound, sf::View &view, bool hassound)
{

    if (e.type == sf::Event::MouseButtonPressed)
    {
        if (e.mouseButton.button == sf::Mouse::Left)
        {
            bool i = findGlobal(w, pausebutton, view);
            if (i)
            {
                updateSound(sound, hassound);
                isPause = true;
                isGame = false;
            }

            if (isPause)
            {
                if (pauseclose.contains(sf::Vector2f(sf::Mouse::getPosition(w))))
                {
                    updateSound(sound, hassound);
                    isPause = false;
                    isGame = true;
                }
                int i = findGlobal(w, pauseoption, view);
                if (i == 0)
                {
                    updateSound(sound, hassound);
                    isPause = false;
                    isGame = true;
                }
                if (i == 1)
                {
                    updateSound(sound, hassound);
                }
                if (i == 2)
                {
                    updateSound(sound, hassound);
                    clickE++;
                }
            }
        }
    }
}
bool Game::backtolobby()
{
    if (clickE == 1)
    {
        background[0] = sf::Vertex(sf::Vector2f(-10, 0), sf::Color(255, 255, 255), sf::Vector2f(0, 0));
        background[1] = sf::Vertex(sf::Vector2f(winX + 10, 0), sf::Color(255, 255, 255), sf::Vector2f(tbgX, 0));
        background[2] = sf::Vertex(sf::Vector2f(winX + 10, winY), sf::Color(255, 255, 255), sf::Vector2f(tbgX, tbgY));
        background[3] = sf::Vertex(sf::Vector2f(-10, winY), sf::Color(255, 255, 255), sf::Vector2f(0, tbgY));
        isPause = false;
        isGame = true;
        isGamerun = false;
        clickE = 0;
        return true;
    }
    return false;
}

void Game::pause(sf::RenderWindow &w, sf::View &view)
{
    pausescreen.setOrigin(pausepopup.getSize().x / 2, pausepopup.getSize().y / 2);
    pausescreen.setPosition(winX / 2 + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, winY / 2);

    for (int i = 0; i < 3; i++)
    {
        pauseoption[i].setOrigin(pauseT[i].getSize().x / 2, pauseT[i].getSize().y / 2);
        pauseoption[i].setPosition(winX / 2 + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, 70 + 70 * i + (3 + (1.5 * i)) * (((winY - pausescreen.getOrigin().y * 2) / 2)));
    }
}

void Game::setPause(bool pauseopt)
{
    isPause = pauseopt;
}

// sound
void Game::playSound(sf::Sound &sound, bool hassound)
{
    if (hassound)
        sound.play();
}
void Game::stopSound(sf::Sound &sound, bool hassound)
{
    if (hassound == false && sound.getStatus() == sf::SoundSource::Playing)
        sound.stop();
}
void Game::updateSound(sf::Sound &sound, bool hassound)
{
    playSound(sound, hassound);
    stopSound(sound, hassound);
}
void Game::draw(sf::RenderWindow &w, sf::View &view, sf::Sound &sound, bool hassound)
{

    w.draw(&background[0], background.size(), sf::TriangleStrip, &tbg);
    m_world.render(w);
    if (isGame)
    {
        w.draw(pausebutton);
    }
    if (isPause)
    {
        w.draw(blurbg);
        w.draw(pausescreen);
        for (int i = 0; i < 3; i++)
        {
            w.draw(pauseoption[i]);
        }
    }
}
void Game::update(sf::RenderWindow &w, sf::View &view, float dt)
{
    if (isPause)
    {
        pause(w, view);
        pausebutton.setPosition(winX - pausebutton.getOrigin().x + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, pausebutton.getPosition().y);
    }
    else
    {
        m_world.update(w, view, dt);
        pausebutton.setPosition(winX - pausebutton.getOrigin().x + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, pausebutton.getPosition().y);
        blurbg.setPosition(blurbg.getOrigin().x + w.mapPixelToCoords(sf::Vector2i(0, 0), view).x, blurbg.getOrigin().y);
        m_world.syncview(view);
    }
}
Game::~Game()
{
}
void Game::movebg(float movement)
{
    background[0].position += sf::Vector2f(movement, 0);
    background[1].position += sf::Vector2f(movement, 0);
    background[2].position += sf::Vector2f(movement, 0);
    background[3].position += sf::Vector2f(movement, 0);
}
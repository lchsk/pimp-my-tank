#include "tank.h"

namespace ks
{
    Tank::Tank(std::unique_ptr<sf::Texture>& tex, int side, int x, int y)
        : _side(side)
    {
        _tank = std::make_unique<sf::Sprite>(*tex.get());

        _tank->setPosition(x, y);
        _tank->setColor(sf::Color::Blue);
    }

    Tank::~Tank()
    {

    }

    void Tank::render(sf::RenderWindow& window)
    {
        window.draw(*_tank);
    }

}

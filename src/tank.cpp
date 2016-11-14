#include "tank.h"

namespace pmt
{
    Tank::Tank(
        std::unique_ptr<sf::Texture>& tank,
        std::unique_ptr<sf::Texture>& gun,
        int side, bool left, int x, int y)
        : _side(side),
          _left(left)
    {
        _tank = std::make_unique<sf::Sprite>(*tank.get());
        _gun = std::make_unique<sf::Sprite>(*gun.get());

        _tank->setPosition(x, y);


        if (left) {
            _tank->setScale(-1, 1);
            _gun->setScale(-1, 1);

            _gun->setPosition(x - 20, y + 6);
            _gun->setRotation(10);

        } else {
            _gun->setPosition(x + 20, y + 6);
            _gun->setRotation(-10);

        }

        // _tank->setColor(sf::Color::Blue);
    }

    Tank::~Tank()
    {

    }

    void Tank::render(sf::RenderWindow& window)
    {
        window.draw(*_gun);
        window.draw(*_tank);
    }

}

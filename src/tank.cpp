#include <iostream>

#include "tank.h"

namespace pmt
{
    Tank::Tank(
        std::unique_ptr<sf::Texture>& tank,
        std::unique_ptr<sf::Texture>& gun,
        int side, bool left, int x, int y)
        : _side(side),
          _left(left),
          _gun_rotation(7.0f)
    {
        _tank = std::make_unique<sf::Sprite>(*tank.get());
        _gun = std::make_unique<sf::Sprite>(*gun.get());

        _tank->setPosition(x, y);

        if (left) {
            _tank->setScale(-1, 1);
            _gun->setScale(-1, 1);

            _gun->setPosition(x - 20, y + 6);

        } else {
            _gun->setPosition(x + 20, y + 6);
        }

        _rotate_gun(0);

        // _tank->setColor(sf::Color::Blue);
    }

    void Tank::gun_up()
    {
        _rotate_gun(ROTATION_UNIT);
    }

    void Tank::gun_down()
    {
        _rotate_gun(-ROTATION_UNIT);
    }

    Tank::~Tank()
    {
    }

    void Tank::render(sf::RenderWindow& window)
    {
        window.draw(*_gun);
        window.draw(*_tank);
    }

    void Tank::_rotate_gun(double val)
    {
        int next_val = static_cast<int>(_gun_rotation + val) % 360;

        if (((next_val >= -15 && next_val < 87)))
            _gun_rotation += val;

        if (_left)
            _gun->setRotation(_gun_rotation);
        else
            _gun->setRotation(-_gun_rotation);
    }
}

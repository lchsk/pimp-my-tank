#include <iostream>

#include "tank.h"
#include "util.h"

namespace pmt
{
    Tank::Tank(
        std::shared_ptr<pmt::BulletMgr>& bullet_mgr,
        std::unique_ptr<sf::Texture>& tank,
        std::unique_ptr<sf::Texture>& gun,
        std::unique_ptr<sf::Texture>& green,
        std::unique_ptr<sf::Texture>& red,
        std::unique_ptr<sf::Texture>& shield,
        int side, bool left, int x, int y)
        : _side(side),
          _left(left),
          _health(100),
          _shield(100),
          _gun_rotation(7.0f),
          _current_weapon(WeaponType::Missile)
    {
        _tank = std::make_unique<sf::Sprite>(*tank.get());
        _gun = std::make_unique<sf::Sprite>(*gun.get());

        _bullet_mgr = bullet_mgr;

        if (left) {
            _tank->setScale(-1, 1);
            _gun->setScale(-1, 1);

            _gun->setPosition(x - 20, y + 6);

        } else {
            _gun->setPosition(x + 20, y + 6);
        }

        _tank->setPosition(x, y);

        int tank_width = tank->getSize().x * _tank->getScale().x;
        int tank_middle_x = x + tank_width / 2 - 10;

        int health_diff = -7;
        int shield_diff = -10;

        for (int i = 0; i < 10; i++) {
            _greens.push_back(std::make_unique<sf::Sprite>(*green.get()));
            _greens[i]->setPosition(tank_middle_x + i * 2, y + health_diff);

            _reds.push_back(std::make_unique<sf::Sprite>(*red.get()));
            _reds[i]->setPosition(tank_middle_x + i * 2, y + health_diff);

            _shields.push_back(std::make_unique<sf::Sprite>(*shield.get()));
            _shields[i]->setPosition(tank_middle_x + i * 2, y + shield_diff);
        }

        // Set initial gun rotation
        _rotate_gun(0);

        // Set initial weapons
        _weapons[WeaponType::Missile] = 5;

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

    void Tank::shoot()
    {
        switch(_current_weapon) {
        case WeaponType::Missile:
            _bullet_mgr->shoot(
                _left,
                _current_weapon,
                _gun_rotation,
                80,
                _gun->getPosition().x,
                _gun->getPosition().y);

            break;
        }
    }

    Tank::~Tank()
    {
    }

    void Tank::render(sf::RenderWindow& window)
    {
        window.draw(*_gun);
        window.draw(*_tank);

        _render_health(window);
        _render_shield(window);
    }

    void Tank::_render_health(sf::RenderWindow& window)
    {
        int g_cnt = ceil(_health / 10.0);

        for (int i = 0; i < g_cnt; i++)
            window.draw(*_greens[i]);

        for (int i = g_cnt; i < 10; i++)
            window.draw(*_reds[i]);
    }

    void Tank::_render_shield(sf::RenderWindow& window)
    {
        int g_sh = ceil(_shield / 10.0);

        for (int i = 0; i < g_sh; i++)
            window.draw(*_shields[i]);
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

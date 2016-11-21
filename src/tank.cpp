#include <iostream>

#include "tank.h"
#include "util.h"

namespace pmt
{
    Tank::Tank(
        unsigned tank_id,
        std::shared_ptr<pmt::BulletMgr>& bullet_mgr,
        std::unique_ptr<sf::Texture>& tank,
        std::unique_ptr<sf::Texture>& gun,
        std::unique_ptr<sf::Texture>& green,
        std::unique_ptr<sf::Texture>& red,
        std::unique_ptr<sf::Texture>& shield,
        std::shared_ptr<sf::Font>& font,
        GameSide side, bool left, int x, int y)
        : _side(side),
          _left(left),
          _has_turn(false),
          _health(100),
          _shield(100),
          _cash(0),
          _tank_id(tank_id),
          _gun_rotation(7.0f),
          _current_weapon(WeaponType::Missile)
    {
        _tank = std::make_unique<sf::Sprite>(*tank.get());
        _gun = std::make_unique<sf::Sprite>(*gun.get());

        _bullet_mgr = bullet_mgr;
        _font = font;

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

    bool Tank::is_human() const
    {
        return _side == GameSide::Human;
    }

    bool Tank::is_alive() const
    {
        return _health > 0;
    }

    bool Tank::has_turn() const
    {
        return _has_turn;
    }

    int Tank::get_id()
    {
        return _tank_id;
    }

    int Tank::get_cash() const
    {
        return _cash;
    }

    void Tank::gun_up()
    {
        _rotate_gun(ROTATION_UNIT);
    }

    void Tank::gun_down()
    {
        _rotate_gun(-ROTATION_UNIT);
    }

    void Tank::activate()
    {
        _has_turn = true;
    }

    void Tank::deactivate()
    {
        _has_turn = false;
    }

    void Tank::add_cash(unsigned amount)
    {
        _cash += amount;
    }

    void Tank::shoot()
    {
        switch(_current_weapon) {
        case WeaponType::Missile:
            _bullet_mgr->shoot(
                _tank_id,
                _left,
                _current_weapon,
                _gun_rotation,
                120,
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

    bool Tank::check_collision(std::shared_ptr<pmt::Bullet>& bullet)
    {
        if (bullet->is_flying()
            && _tank->getGlobalBounds().intersects(
                bullet->get_sprite()->getGlobalBounds())
            // Do not hit yourself
            && _tank_id != bullet->get_origin_tank()) {
            _hit(bullet);

            // Cash for being hit
            _cash += pmt::config::REWARD_BEING_HIT;

            return true;
        }

        return false;
    }

    void Tank::_hit(std::shared_ptr<pmt::Bullet>& bullet)
    {
        bullet->hit();

        switch(bullet->get_type()) {
        case WeaponType::Missile:
            if (_shield > 0)
                _shield -= 24;

            if (_shield <= 0)
                _health -= 20;

            break;

        case WeaponType::MagnumMissile:
            if (_shield > 0) {
                _shield -= 36;
                _health -= 9;
            }

            if (_shield <= 0)
                _health -= 25;

            break;
        }
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

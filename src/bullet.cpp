#include "bullet.h"

namespace pmt
{
    Bullet::Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture) :
        _flying(false),
        _type(type),
        _flying_time(0)
    {
        _sprite = std::make_unique<sf::Sprite>(*texture);
    }

    Bullet::~Bullet()
    {
    }

    void Bullet::render(sf::RenderWindow& window)
    {
        window.draw(*_sprite);
    }

    bool Bullet::is_flying() const
    {
        return _flying;
    }

    bool Bullet::is_flying_left() const
    {
        return _flying_left;
    }

    double Bullet::get_angle()
    {
        return _angle;
    }

    double Bullet::get_initial_speed()
    {
        return _initial_speed;
    }

    double Bullet::get_start_x()
    {
        return _start_x;
    }

    double Bullet::get_start_y()
    {
        return _start_y;
    }

    WeaponType Bullet::get_type() const
    {
        return _type;
    }

    unsigned Bullet::get_origin_tank() const
    {
        return _origin_tank;
    }

    std::unique_ptr<sf::Sprite>& Bullet::get_sprite()
    {
        return _sprite;
    }

    void Bullet::set_position(double x, double y)
    {
        _sprite->setPosition(x, y);
    }

    void Bullet::shoot(unsigned origin_tank,
                       bool shoot_left,
                       double angle,
                       double initial_speed,
                       double start_x,
                       double start_y)
    {
        _flying = true;
        _flying_left = shoot_left;
        _origin_tank = origin_tank;

        _sprite->setRotation(shoot_left ? -angle + 180 : angle);
        _angle = -angle;
        _initial_speed = initial_speed;
        _start_x = start_x;
        _start_y = start_y;
    }

    void Bullet::hit()
    {
        _flying = false;
        _flying_time = 0;
    }

    void Bullet::increase_flying_time(double time)
    {
        _flying_time += pmt::config::MISSILE_SPEED * time;
    }

    double Bullet::get_time()
    {
        return _flying_time;
    }
}

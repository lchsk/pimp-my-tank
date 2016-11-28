#include <iostream>

#include "bullet.h"

namespace pmt
{
    Bullet::Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture) :
        _flying(false),
        _type(type),
        _flying_time(0)
    {
        switch (type) {
        case WeaponType::Missile:
            _mass = 120;
            break;

        case WeaponType::MagnumMissile:
            _mass = 300;
            break;

        case WeaponType::Sheep:
            _mass = 50;
            break;
        }

        _sprite = std::make_unique<sf::Sprite>(*texture);
    }

    Bullet::~Bullet()
    {
    }

    void Bullet::simulate(sf::Time delta, double wind)
    {
        double g = pmt::config::G;
        double Gamma_wind = pmt::config::GAMMA_WIND;
        double C_air = pmt::config::C_AIR;
        double C_wind = pmt::config::C_WIND;
        double V_wind = wind * pmt::config::WIND_POWER_COEF;
        double mass = get_mass();

        double cosX, cosY;
        double b, Lx, Ly;
        double sx1, vx1;
        double sy1, vy1;

        double Vm = get_initial_speed();

        double Alpha, Gamma;
        Alpha = 90 - get_angle();
        Gamma = get_angle();

        increase_flying_time(delta.asSeconds());

        b = 10.0 * cos(pmt::util::radian(90-Alpha) );
        Lx = b * cos(pmt::util::radian(Gamma));

        Ly = 10.0 * cos(pmt::util::radian(Alpha));

        cosX = Lx / 10;
        cosY = Ly / 10;

        sx1 = get_start_x();
        sy1 = get_start_y();

        vx1 = Vm * cosX;

        if (! is_flying_left())
            vx1 *= -1;

        // sy1 = 300.0 * cos(pmt::util::radian(Alpha)); //wspolrzedna y konca lufy
        vy1 = Vm * cosY; //skladowa vy predkosci

        double time = get_time();

        double x, y;
        x = ( (mass/C_air) * exp(-(C_air * time)/mass) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1) - (C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)) * time)/C_air ) - ( (mass/C_air) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1)) + sx1;
        y = (sy1 + ( -(vy1 + (mass*g)/C_air) * (mass/C_air) * exp(-(C_air*time)/mass) - (mass * g * time)/C_air) + ( (mass/C_air) * (vy1 + (mass * g)/C_air)));

        set_position(x, pmt::config::WINDOW_H - y);
    }

    bool Bullet::on_screen() const
    {
        sf::Vector2f pos = get_position();

        if (pos.x < 0
            || pos.x > pmt::config::WINDOW_W
            || pos.y < 0
            || pos.y > pmt::config::WINDOW_H)
            return false;

        return true;
    }

    void Bullet::render(sf::RenderWindow& window)
    {
        window.draw(*_sprite);
    }

    double Bullet::get_mass() const
    {
        return _mass;
    }

    sf::Vector2f Bullet::get_position() const
    {
        return _sprite->getPosition();
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

        switch (_type) {
        case WeaponType::Missile:
        case WeaponType::MagnumMissile:
            _sprite->setRotation(shoot_left ? -angle + 180 : angle);

            break;

        case WeaponType::Sheep:

            break;
        }

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

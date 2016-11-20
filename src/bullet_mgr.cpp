#include <iostream>

#include "bullet_mgr.h"
#include "util.h"

namespace pmt
{
    Bullet::Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture) :
        _flying(false),
        _type(type)
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

    void Bullet::set_position(double x, double y)
    {
        _sprite->setPosition(x, y);
    }

    void Bullet::shoot(bool shoot_left,
                       double angle,
                       double initial_speed,
                       double start_x,
                       double start_y)
    {
        _flying = true;
        _flying_left = shoot_left;

        _sprite->setRotation(shoot_left ? -angle + 180 : angle);
        _angle = -angle;
        _initial_speed = initial_speed;
        _start_x = start_x;
        _start_y = start_y;
    }

    BulletMgr::BulletMgr()
    {
    }

    BulletMgr::~BulletMgr()
    {
    }

    void BulletMgr::add_bullets(
        WeaponType type,
        unsigned count,
        std::unique_ptr<sf::Texture>& texture)
    {
        for (unsigned i = 0; i < count; i++)
            _bullets[type].push_back(std::make_shared<Bullet>(type, texture));
    }

    void BulletMgr::shoot(bool shoot_left,
                          WeaponType type,
                          double angle,
                          double initial_speed,
                          double start_x,
                          double start_y)
    {
        for (auto& bullet : _bullets[type]) {
            if (! bullet->is_flying()) {
                std::cout << "angle: " << angle << "\n";

                double x = start_x;
                double y = pmt::config::WINDOW_H - start_y;

                x += 24;
                y -= 24;

                x = x * cos(pmt::util::radian(angle));
                // double p2 = (bullet->start_y) * sin(pmt::util::radian(angle));

                // double xp = p1;

                y = x * sin(pmt::util::radian(angle))
                    + y * cos(pmt::util::radian(angle));

                // x = xp;

                // TODO:
                bullet->shoot(
                    shoot_left,
                    -angle,
                    initial_speed,
                    start_x,
                    pmt::config::WINDOW_H - start_y);
            }
        }
    }

    void BulletMgr::update(sf::Time delta)
    {
        for (auto it = _bullets.begin(); it != _bullets.end(); ++it) {
            for (auto& bullet : it->second) {
                if (bullet->is_flying()) {
                    _simulate(delta, bullet);
                }
            }
        }
    }

    void BulletMgr::render(sf::RenderWindow& window)
    {
        for (auto it = _bullets.begin(); it != _bullets.end(); ++it) {
            for (auto& bullet : it->second) {
                if (bullet->is_flying()) {
                    bullet->render(window);
                }
            }
        }
    }

    void BulletMgr::_simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet)
    {
        double g = 9.81;
        double Gamma_wind = 0.0;
        double C_air = 1.0;
        double C_wind = 50.0;
        double V_wind = 0.0;
        double mass = 100.0;

        double cosX, cosY;
        double b, Lx, Ly;
        double sx1, vx1;
        double sy1, vy1;

        static double time;

        double Vm = bullet->get_initial_speed();

        double Alpha, Gamma;
        Alpha = 90 - bullet->get_angle();
        Gamma = bullet->get_angle();

        time += pmt::config::MISSILE_SPEED * delta.asSeconds();

        b = 10.0 * cos(pmt::util::radian(90-Alpha) );
        Lx = b * cos(pmt::util::radian(Gamma));

        Ly = 10.0 * cos(pmt::util::radian(Alpha));

        cosX = Lx / 10;
        cosY = Ly / 10;

        sx1 = bullet->get_start_x();
        sy1 = bullet->get_start_y();

        vx1 = Vm * cosX;

        if (! bullet->is_flying_left())
            vx1 *= -1;

        // sy1 = 300.0 * cos(pmt::util::radian(Alpha)); //wspolrzedna y konca lufy
        vy1 = Vm * cosY; //skladowa vy predkosci

        double x, y;
        x = ( (mass/C_air) * exp(-(C_air*time)/mass) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1) - (C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)) * time)/C_air ) - ( (mass/C_air) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1)) + sx1;
        y = (sy1 + ( -(vy1 + (mass*g)/C_air) * (mass/C_air) * exp(-(C_air*time)/mass) - (mass * g * time)/C_air) + ( (mass/C_air) * (vy1 + (mass * g)/C_air)));

        bullet->set_position(x, pmt::config::WINDOW_H - y);
    }
}

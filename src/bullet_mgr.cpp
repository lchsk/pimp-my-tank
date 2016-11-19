#include <iostream>

#include "bullet_mgr.h"
#include "util.h"

namespace pmt
{
    Bullet::Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture) :
        _flying(true),
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

    void Bullet::set_position(double x, double y)
    {
        _sprite->setPosition(x, y);
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

    void BulletMgr::shoot(WeaponType type)
    {

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
        double xe;
			// , ze;
        double b, Lx, Ly;
        double sx1, vx1;
        double sy1, vy1;
        // double tx1, tx2;
        // double ty1, ty2;
        static double time;

        double Vm = 80;

        double Alpha, Gamma;
        Alpha = 50;
        Gamma = 20;

        time += pmt::config::MISSILE_SPEED * delta.asSeconds();

        b = 10.0 * cos(pmt::util::radian(90-Alpha) );
        Lx = b * cos(pmt::util::radian(Gamma));

        Ly = 10.0 * cos(pmt::util::radian(Alpha));

        cosX = Lx / 10;
        cosY = Ly / 10;

        xe = 10 * cos(pmt::util::radian(90-Alpha) ) * cos(pmt::util::radian(Gamma) );

        sx1 = xe; //wspolrzednia x konca lufy
        vx1 = Vm * cosX; //skadlowa vx predkosci
        sy1 = 350.0 * cos(pmt::util::radian(Alpha)); //wspolrzedna y konca lufy
        vy1 = Vm * cosY; //skladowa vy predkosci

        //obliczanie wspolrzendych pocisku
        double x, y;
        x = ( (mass/C_air) * exp(-(C_air*time)/mass) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air - vx1) - (C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)) * time)/C_air ) - ( (mass/C_air) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air - vx1)) + sx1;
        y = (sy1 + ( -(vy1 + (mass*g)/C_air) * (mass/C_air) * exp(-(C_air*time)/mass) - (mass * g * time)/C_air) + ( (mass/C_air) * (vy1 + (mass * g)/C_air)));

        bullet->set_position(x, pmt::config::WINDOW_H - y);
    }
}

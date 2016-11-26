#include <iostream>

#include "bullet_mgr.h"
#include "util.h"

namespace pmt
{
    BulletMgr::BulletMgr(std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
               textures) :
        _wind(0),
        _wind_change_timer(0)
    {
        _weapon_names = {
            {WeaponType::Missile, "Missile"},
            {WeaponType::MagnumMissile, "Magnum Missile"},
        };

        _urd = std::make_unique<std::uniform_real_distribution<double> >(-0.5, 0.5);
        _rd = std::make_unique<std::random_device>();
        _mt = std::make_unique<std::mt19937>((*_rd)());
    }

    BulletMgr::~BulletMgr()
    {
    }

    std::string BulletMgr::get_weapon_name(const WeaponType type)
    {
        return _weapon_names[type];
    }

    void BulletMgr::add_bullets(
        WeaponType type,
        unsigned count,
        std::unique_ptr<sf::Texture>& texture)
    {
        for (unsigned i = 0; i < count; i++)
            _bullets[type].push_back(std::make_shared<Bullet>(type, texture));
    }

    void BulletMgr::shoot(unsigned origin_tank,
                          bool shoot_left,
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
                    origin_tank,
                    shoot_left,
                    -angle,
                    initial_speed,
                    start_x,
                    pmt::config::WINDOW_H - start_y);
            }
        }
    }

    std::vector<std::shared_ptr<pmt::Bullet> >&
    BulletMgr::get_flying_bullets()
    {
        for (auto it = _bullets.begin(); it != _bullets.end(); ++it)
            for (auto& bullet : it->second)
                if (bullet->is_flying())
                    _vec.push_back(bullet);

        return _vec;
    }

    void BulletMgr::update(sf::Time delta)
    {
        _update_wind(delta);

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

    double BulletMgr::get_wind() const
    {
        return _wind;
    }

    void BulletMgr::_update_wind(sf::Time delta)
    {
        // Don't update wind if a bullet is flying
        for (auto it = _bullets.begin(); it != _bullets.end(); ++it)
            for (auto& bullet : it->second)
                if (bullet->is_flying())
                    return;

        _wind_change_timer += delta.asMilliseconds();

        if (_wind_change_timer > 1500) {
            std::cout << "CHANGE WIND\n";

            _wind += _get_random();
            std::cout << "WIND: " << _wind << "\n";
            _wind_change_timer = 0;

        }
    }

    double BulletMgr::_get_random()
    {
        return (*_urd)(*_mt);
    }

    void BulletMgr::_simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet)
    {
        double g = pmt::config::G;
        double Gamma_wind = pmt::config::GAMMA_WIND;
        double C_air = pmt::config::C_AIR;
        double C_wind = pmt::config::C_WIND;
        double V_wind = _wind * pmt::config::WIND_POWER_COEF;
        double mass = 200.0;

        double cosX, cosY;
        double b, Lx, Ly;
        double sx1, vx1;
        double sy1, vy1;

        double Vm = bullet->get_initial_speed();

        double Alpha, Gamma;
        Alpha = 90 - bullet->get_angle();
        Gamma = bullet->get_angle();

        bullet->increase_flying_time(delta.asSeconds());

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

        double time = bullet->get_time();

        double x, y;
        x = ( (mass/C_air) * exp(-(C_air * time)/mass) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1) - (C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)) * time)/C_air ) - ( (mass/C_air) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air + vx1)) + sx1;
        y = (sy1 + ( -(vy1 + (mass*g)/C_air) * (mass/C_air) * exp(-(C_air*time)/mass) - (mass * g * time)/C_air) + ( (mass/C_air) * (vy1 + (mass * g)/C_air)));

        bullet->set_position(x, pmt::config::WINDOW_H - y);
    }
}

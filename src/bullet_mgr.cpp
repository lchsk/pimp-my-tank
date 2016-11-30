#include <iostream>

#include "bullet_mgr.h"
#include "util.h"

namespace pmt
{
    BulletMgr::BulletMgr(std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
                         textures) :
        _wind(0),
        _wind_change_timer(0),
        _wind_change_period(2000)
    {
        _weapon_names = {
            {WeaponType::Missile, "Missile"},
            {WeaponType::MagnumMissile, "Magnum Missile"},
            {WeaponType::Sheep, "Sheep"},
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
                double offset = 0;

                switch (type) {
                case WeaponType::Missile:
                case WeaponType::MagnumMissile:
                    offset = -5;
                    break;

                case WeaponType::Sheep:
                    offset = 20;
                    break;
                }

                bullet->shoot(
                    origin_tank,
                    shoot_left,
                    -angle,
                    initial_speed,
                    start_x,
                    pmt::config::WINDOW_H - start_y + offset);
            }
        }
    }

    std::vector<std::shared_ptr<pmt::Bullet> >&
    BulletMgr::get_flying_bullets()
    {
        _vec.clear();

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

    void BulletMgr::set_wind(double wind)
    {
        _wind = wind;
    }

    void BulletMgr::set_wind_change_period(double wind_change_period)
    {
        _wind_change_period = wind_change_period;
    }

    void BulletMgr::_update_wind(sf::Time delta)
    {
        // Don't update wind if a bullet is flying
        for (auto it = _bullets.begin(); it != _bullets.end(); ++it)
            for (auto& bullet : it->second)
                if (bullet->is_flying())
                    return;

        _wind_change_timer += delta.asMilliseconds();

        if (_wind_change_timer > _wind_change_period) {
            _wind += _get_random();
            _wind_change_timer = 0;
        }
    }

    double BulletMgr::_get_random()
    {
        return (*_urd)(*_mt);
    }

    void BulletMgr::_simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet)
    {
        bullet->simulate(delta, _wind);
    }
}

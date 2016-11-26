#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "animation.h"
#include "bullet.h"

namespace pmt
{
    static std::vector<WeaponType> WeaponsOrder = {
        WeaponType::Missile,
        WeaponType::MagnumMissile
    };

    class BulletMgr
    {
        public:
            BulletMgr(const BulletMgr&) = delete;
            BulletMgr& operator=(const BulletMgr&) = delete;

            BulletMgr(std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
                      textures, double wind);
            ~BulletMgr();

            void update(sf::Time delta);
            void render(sf::RenderWindow& window);
            void add_bullets(WeaponType type,
                             unsigned count,
                             std::unique_ptr<sf::Texture>& texture);

            void shoot(unsigned origin_tank,
                       bool shoot_left,
                       WeaponType type,
                       double angle,
                       double initial_speed,
                       double start_x,
                       double start_y);

            std::vector<std::shared_ptr<pmt::Bullet> >&
                get_flying_bullets();

            std::string get_weapon_name(const WeaponType type);
            double get_wind() const;

        private:
            void _simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet);
            void _update_wind(sf::Time delta);
            double _get_random();

            std::unordered_map
                <WeaponType, std::vector<std::shared_ptr<pmt::Bullet> > >
                _bullets;

            std::vector<std::shared_ptr<pmt::Bullet> > _vec;

            std::unordered_map<WeaponType, std::string> _weapon_names;

            double _wind;
            double _wind_change_timer;

            std::unique_ptr<std::uniform_real_distribution<double> > _urd;
            std::unique_ptr<std::random_device> _rd;
            std::unique_ptr<std::mt19937> _mt;
    };
}

#endif

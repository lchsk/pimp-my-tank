#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace pmt
{
    enum class WeaponType {
        Missile,
        MagnumMissile
    };

    class Bullet
    {
    public:
        Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture);
        ~Bullet();

        bool is_flying() const;
        double get_angle();
        double get_initial_speed();
        double get_start_x();
        double get_start_y();

        void render(sf::RenderWindow& window);
        void set_position(double x, double y);
        void shoot(double angle,
                   double initial_speed,
                   double start_x,
                   double start_y);


    private:
        std::unique_ptr<sf::Sprite> _sprite;
        bool _flying;
        WeaponType _type;

        double _angle;
        double _initial_speed;
        double _start_x;
        double _start_y;
    };

    class BulletMgr
    {
        public:
            BulletMgr(const BulletMgr&) = delete;
            BulletMgr& operator=(const BulletMgr&) = delete;

            BulletMgr();
            ~BulletMgr();

            void update(sf::Time delta);
            void render(sf::RenderWindow& window);
            void add_bullets(WeaponType type,
                             unsigned count,
                             std::unique_ptr<sf::Texture>& texture);

            void shoot(WeaponType type,
                       double angle,
                       double initial_speed,
                       double start_x,
                       double start_y);

        private:
            void _simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet);

            std::unordered_map
                <WeaponType, std::vector<std::shared_ptr<pmt::Bullet> > >
                _bullets;
    };
}

#endif

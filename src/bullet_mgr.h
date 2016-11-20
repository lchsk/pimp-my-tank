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
        bool is_flying_left() const;
        double get_angle();
        double get_initial_speed();
        double get_start_x();
        double get_start_y();
        WeaponType get_type() const;
        unsigned get_origin_tank() const;
        std::unique_ptr<sf::Sprite>& get_sprite();

        void render(sf::RenderWindow& window);
        void set_position(double x, double y);
        void shoot(unsigned origin_tank,
                   bool shoot_left,
                   double angle,
                   double initial_speed,
                   double start_x,
                   double start_y);

        void hit();

    private:
        std::unique_ptr<sf::Sprite> _sprite;

        // Is flying or not
        bool _flying;

        // Flying left or right
        bool _flying_left;

        unsigned _origin_tank;

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

            void shoot(unsigned origin_tank,
                       bool shoot_left,
                       WeaponType type,
                       double angle,
                       double initial_speed,
                       double start_x,
                       double start_y);

            std::vector<std::shared_ptr<pmt::Bullet> >&
                get_flying_bullets();

        private:
            void _simulate(sf::Time delta, std::shared_ptr<pmt::Bullet> bullet);

            std::unordered_map
                <WeaponType, std::vector<std::shared_ptr<pmt::Bullet> > >
                _bullets;

            std::vector<std::shared_ptr<pmt::Bullet> > _vec;
    };
}

#endif

#ifndef BULLET_H
#define BULLET_H

#include "util.h"

namespace pmt
{
    class Bullet
    {
    public:
        Bullet(WeaponType type, std::unique_ptr<sf::Texture>& texture);
        ~Bullet();

        bool is_flying() const;
        bool is_flying_left() const;
        bool on_screen() const;
        double get_angle();
        double get_initial_speed();
        double get_start_x();
        double get_start_y();
        double get_mass() const;
        sf::Vector2f get_position() const;
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
        void increase_flying_time(double time);
        double get_time();

        void simulate(sf::Time delta, double wind);

    private:

        std::unique_ptr<sf::Sprite> _sprite;

        // Is flying or not
        bool _flying;

        // Flying left or right
        bool _flying_left;

        unsigned _origin_tank;

        WeaponType _type;

        double _mass;

        double _angle;
        double _initial_speed;
        double _start_x;
        double _start_y;

        double _flying_time;
    };
}

#endif

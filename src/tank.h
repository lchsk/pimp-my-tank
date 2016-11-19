#ifndef TANK_H
#define TANK_H

#include <memory>

#include <SFML/Graphics.hpp>

namespace pmt
{
    const double ROTATION_UNIT = 0.5f;

    class Tank
    {
    public:
        Tank(
            std::unique_ptr<sf::Texture>& tank,
            std::unique_ptr<sf::Texture>& gun,
            int side, bool left, int x, int y);
        ~Tank();

        void render(sf::RenderWindow& window);

        void gun_up();
        void gun_down();

    private:
        void _rotate_gun(double val);

        std::unique_ptr<sf::Sprite> _tank;
        std::unique_ptr<sf::Sprite> _gun;
        int _side;
        bool _left;

        double _gun_rotation;
    };
}

#endif

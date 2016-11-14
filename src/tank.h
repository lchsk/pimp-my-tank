#ifndef TANK_H
#define TANK_H

#include <memory>

#include <SFML/Graphics.hpp>

namespace pmt
{
    class Tank
    {
    public:
        Tank(
            std::unique_ptr<sf::Texture>& tank,
            std::unique_ptr<sf::Texture>& gun,
            int side, bool left, int x, int y);
        ~Tank();

        void render(sf::RenderWindow& window);

    private:
        std::unique_ptr<sf::Sprite> _tank;
        std::unique_ptr<sf::Sprite> _gun;
        int _side;
        bool _left;
    };
}

#endif

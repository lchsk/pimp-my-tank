#ifndef TANK_H
#define TANK_H

#include <memory>

#include <SFML/Graphics.hpp>

namespace pmt
{
    class Tank
    {
    public:
        Tank(std::unique_ptr<sf::Texture>& tex, int side, int x, int y);
        ~Tank();

        void render(sf::RenderWindow& window);

    private:
        std::unique_ptr<sf::Sprite> _tank;
        int _side;
    };
}

#endif

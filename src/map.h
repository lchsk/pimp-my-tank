#ifndef MAP_H
#define MAP_H

#include <memory>

#include <SFML/Graphics.hpp>

namespace ks
{
    class Map
    {
    public:
        Map();
        ~Map();

        void render(sf::RenderWindow& window);

    private:
    };
}

#endif

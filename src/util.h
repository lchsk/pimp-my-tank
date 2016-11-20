#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

namespace pmt
{
    namespace config
    {
        const unsigned WINDOW_W = 800;
        const unsigned WINDOW_H = 448;

        const double MISSILE_SPEED = 2.5f;
    }

    namespace util
    {
        const double PI = 3.14159265359;

        inline double radian(double a)
        {
            return a / 180.0 * PI;
        }
    }

    namespace color
    {
        const sf::Color Blue = sf::Color(31, 58, 147, 255);
    }
}

#endif

#ifndef HUD_H
#define HUD_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "tank.h"

namespace pmt
{
    class Hud
    {
    public:
        Hud(std::shared_ptr<sf::Font>& font);
        ~Hud();

        void render(sf::RenderWindow& window);
        void update(sf::Time& delta);

        void update_turn(std::shared_ptr<pmt::Tank>& tank);

    private:
        std::shared_ptr<sf::Font> _font;

        std::unique_ptr<sf::Text> _text_turn_name;
    };
}

#endif

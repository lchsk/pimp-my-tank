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

        void show_turn(std::shared_ptr<pmt::Tank>& tank);
        void show_cash(std::shared_ptr<pmt::Tank>& tank);

    private:
        bool _show_cash;

        std::shared_ptr<sf::Font> _font;

        std::unique_ptr<sf::Text> _text_turn_name;

        std::unique_ptr<sf::Text> _text_cash_label;
        std::unique_ptr<sf::Text> _text_cash;
    };
}

#endif

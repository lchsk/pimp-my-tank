#include <iostream>

#include "hud.h"
#include "util.h"

namespace pmt
{
    Hud::Hud(std::shared_ptr<sf::Font>& font) :
        _font(font)
    {
        _text_turn_name = std::make_unique<sf::Text>();
        _text_turn_name->setFont(*_font);
        _text_turn_name->setCharacterSize(16);
        _text_turn_name->setColor(pmt::color::Blue);
    }

    Hud::~Hud()
    {
    }

    void Hud::render(sf::RenderWindow& window)
    {
        window.draw(*_text_turn_name);
    }

    void Hud::update(sf::Time& delta)
    {
    }

    void Hud::update_turn(std::shared_ptr<pmt::Tank>& tank)
    {
        _text_turn_name->setString(tank->is_human() ? "Your turn" : "AI turn");

        double width = _text_turn_name->getLocalBounds().width;

        _text_turn_name->setPosition(pmt::config::WINDOW_W / 2.0 - width / 2.0, 2);
    }
}

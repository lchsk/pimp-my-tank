#include <iostream>

#include "hud.h"
#include "util.h"

namespace pmt
{
    Hud::Hud(std::shared_ptr<sf::Font>& font) :
        _font(font),
        _show_cash(false)
    {
        _text_turn_name = std::make_unique<sf::Text>();
        _text_turn_name->setFont(*_font);
        _text_turn_name->setCharacterSize(16);
        _text_turn_name->setColor(pmt::color::Blue);

        _text_cash = std::make_unique<sf::Text>();
        _text_cash->setFont(*_font);
        _text_cash->setCharacterSize(20);
    }

    Hud::~Hud()
    {
    }

    void Hud::render(sf::RenderWindow& window)
    {
        window.draw(*_text_turn_name);

        if (_show_cash)
            window.draw(*_text_cash);
    }

    void Hud::update(sf::Time& delta)
    {
    }

    void Hud::show_turn(std::shared_ptr<pmt::Tank>& tank)
    {
        _text_turn_name->setString(tank->is_human() ? "Your turn" : "AI turn");

        double width = _text_turn_name->getLocalBounds().width;

        _text_turn_name->setPosition(pmt::config::WINDOW_W / 2.0 - width / 2.0, 2);
    }

    void Hud::show_cash(std::shared_ptr<pmt::Tank>& tank)
    {
        _show_cash = tank->is_human() && tank->has_turn();

        if (! _show_cash) return;

        _text_cash->setString("$" + std::to_string(tank->get_cash()));

        double width = _text_cash->getLocalBounds().width;

        _text_cash->setPosition(pmt::config::WINDOW_W - width - 5, 2);
        _text_cash->setColor(pmt::color::Blue);

    }
}

#include <iostream>

#include "hud.h"
#include "util.h"

namespace pmt
{
    Hud::Hud(std::shared_ptr<sf::Font>& font) :
        _font(font),
        _show_cash(false),
        _shop_open(false),
        _top_shop_item(0),
        _selected(0)
    {
        _text_turn_name = std::make_unique<sf::Text>();
        _text_turn_name->setFont(*_font);
        _text_turn_name->setCharacterSize(16);
        _text_turn_name->setColor(pmt::color::Blue);

        _text_cash = std::make_unique<sf::Text>();
        _text_cash->setFont(*_font);
        _text_cash->setCharacterSize(20);
        _text_cash->setColor(pmt::color::Blue);

        _text_cash_label = std::make_unique<sf::Text>();
        _text_cash_label->setFont(*_font);
        _text_cash_label->setCharacterSize(14);
        _text_cash_label->setColor(pmt::color::Blue);
        _text_cash_label->setString("Cash");

        _text_weapon = std::make_unique<sf::Text>();
        _text_weapon->setFont(*_font);
        _text_weapon->setCharacterSize(20);
        _text_weapon->setColor(pmt::color::Blue);

		_text_weapon_label = std::make_unique<sf::Text>();
        _text_weapon_label->setFont(*_font);
        _text_weapon_label->setCharacterSize(14);
        _text_weapon_label->setColor(pmt::color::Blue);
        _text_weapon_label->setString("Weapon");

        // Shop

        for (auto& offer: pmt::offers) {
            _shop_names[offer.type] = std::make_unique<sf::Text>();
            _shop_names[offer.type]->setFont(*_font);
            _shop_names[offer.type]->setCharacterSize(18);
            _shop_names[offer.type]->setColor(pmt::color::Blue);
            _shop_names[offer.type]->setString(offer.name);

            _shop_descs[offer.type] = std::make_unique<sf::Text>();
            _shop_descs[offer.type]->setFont(*_font);
            _shop_descs[offer.type]->setCharacterSize(14);
            _shop_descs[offer.type]->setColor(pmt::color::Blue);
            _shop_descs[offer.type]->setString(offer.desc);

            _shop_prices[offer.type] = std::make_unique<sf::Text>();
            _shop_prices[offer.type]->setFont(*_font);
            _shop_prices[offer.type]->setCharacterSize(20);
            _shop_prices[offer.type]->setColor(pmt::color::Blue);
            _shop_prices[offer.type]->setString("$" + std::to_string(offer.price));
        }
    }

    Hud::~Hud()
    {
    }

    void Hud::render(sf::RenderWindow& window)
    {
        window.draw(*_text_turn_name);

        if (_show_cash) {
            window.draw(*_text_cash);
            window.draw(*_text_cash_label);

            window.draw(*_text_weapon);
            window.draw(*_text_weapon_label);
        }

        if (_shop_open) {
            int y = 50;

            for (int i = 0; i < 5; i++) {
                auto& offer = offers[_top_shop_item + i];

                if (_selected == _top_shop_item + i) {
                    _shop_names[offer.type]->setColor(sf::Color::Red);
                } else {
                    _shop_names[offer.type]->setColor(pmt::color::Blue);
                }

                _shop_prices[offer.type]->setPosition(50, y);
                _shop_names[offer.type]->setPosition(100, y);
                _shop_descs[offer.type]->setPosition(100, y + 20);

                window.draw(*_shop_prices[offer.type]);
                window.draw(*_shop_names[offer.type]);
                window.draw(*_shop_descs[offer.type]);

                y += 50;
            }
		}
    }

    void Hud::update(sf::Time& delta)
    {
    }

    bool Hud::is_shop_open() const
    {
        return _shop_open;
    }

    void Hud::shop_up()
    {
        if (_top_shop_item > 0) {
            _top_shop_item--;
        }

        if (_selected > 0)
            _selected--;
    }

    void Hud::shop_down()
    {
        if (_top_shop_item < pmt::offers.size() - 5)
            _top_shop_item++;

        if (_selected < pmt::offers.size() - 1)
            _selected++;
    }

    void Hud::open_shop(std::shared_ptr<pmt::Tank>& tank)
    {
        _shop_client = tank;
        _shop_open = true;
    }

    void Hud::close_shop()
    {
        _shop_open = false;
    }

    void Hud::buy()
    {
        auto& offer = pmt::offers[_selected];

        if (offer.price > _shop_client->get_cash()) {
            std::cout << "No money\n";
        } else {
            _shop_client->buy(offer);
            close_shop();
        }
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

        // Show weapon first
        _text_weapon->setString(tank->get_weapon_name());
        _text_weapon->setPosition(2, 20);
        _text_weapon_label->setPosition(2, 2);

        double weapon_w = std::max(_text_weapon_label->getLocalBounds().width,
                                   _text_weapon->getLocalBounds().width);

        // Cash second
        _text_cash->setString("$" + std::to_string(tank->get_cash()));
        _text_cash_label->setPosition(weapon_w + 20, 2);
        _text_cash->setPosition(weapon_w + 20, 20);
    }
}

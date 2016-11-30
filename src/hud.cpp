#include <iostream>

#include "hud.h"
#include "util.h"

namespace pmt
{
    Hud::Hud(std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
             textures, std::shared_ptr<sf::Font>& font) :
        _show_cash(false),
        _shop_open(false),
        _top_shop_item(0),
        _selected(0),
        _font(font)
    {
        _shop_overlay = std::make_unique<sf::RectangleShape>();
        _shop_overlay->setSize(
            sf::Vector2f(pmt::config::WINDOW_W, pmt::config::WINDOW_H));
        _shop_overlay->setFillColor(pmt::color::Overlay);

        _text_shop_header = std::make_unique<sf::Text>("Shop", *_font, 64);
        _text_shop_header->setColor(pmt::color::Blue);

        _text_shop_info = std::make_unique<sf::Text>("Not enough cash", *_font, 16);
        _text_shop_info->setColor(pmt::color::Red);

        _text_turn_name = std::make_unique<sf::Text>();
        _text_turn_name->setFont(*_font);
        _text_turn_name->setCharacterSize(16);
        _text_turn_name->setColor(pmt::color::Blue);

        _text_cash = std::make_unique<sf::Text>();
        _text_cash->setFont(*_font);
        _text_cash->setCharacterSize(16);
        _text_cash->setColor(pmt::color::Blue);

        _text_weapon = std::make_unique<sf::Text>();
        _text_weapon->setFont(*_font);
        _text_weapon->setCharacterSize(16);
        _text_weapon->setColor(pmt::color::Blue);

        _text_weapon_count = std::make_unique<sf::Text>();
        _text_weapon_count->setFont(*_font);
        _text_weapon_count->setCharacterSize(16);
        _text_weapon_count->setColor(pmt::color::Blue);
        _text_weapon_count->setString("Weapon");

        // Shop

        for (auto& offer: pmt::offers) {
            _shop_names[offer.type] = std::make_unique<sf::Text>();
            _shop_names[offer.type]->setFont(*_font);
            _shop_names[offer.type]->setCharacterSize(16);
            _shop_names[offer.type]->setString(offer.name);

            _shop_descs[offer.type] = std::make_unique<sf::Text>();
            _shop_descs[offer.type]->setFont(*_font);
            _shop_descs[offer.type]->setCharacterSize(16);
            _shop_descs[offer.type]->setString(offer.desc);

            _shop_prices[offer.type] = std::make_unique<sf::Text>();
            _shop_prices[offer.type]->setFont(*_font);
            _shop_prices[offer.type]->setCharacterSize(16);
            _shop_prices[offer.type]->setString("$" + std::to_string(offer.price));
        }

        _pole = std::make_unique<sf::Sprite>(*textures["pole.png"].get());
        _pole->setScale(1.5, 1.5);
        _pole->setPosition(
            pmt::config::WINDOW_W / 2 - textures["pole.png"]->getSize().x / 2,
        0);

        std::vector<std::string> frames = {"flag_1.png", "flag_2.png"};

        _flag = std::make_unique<pmt::Animation>(textures, frames);
        _flag->set_position(pmt::config::WINDOW_W / 2, 43);
        _flag->set_speed(0);
        _flag->loop(true);
        _flag->play();

        _text_wind = std::make_unique<sf::Text>();
        _text_wind->setFont(*_font);
        _text_wind->setCharacterSize(16);
        _text_wind->setColor(pmt::color::Blue);
        _text_wind->setPosition(pmt::config::WINDOW_W / 2 + 30, 30);
    }

    Hud::~Hud()
    {
    }

    void Hud::render(sf::RenderWindow& window)
    {
        window.draw(*_text_turn_name);

        window.draw(*_pole);
        _flag->render(window);

        window.draw(*_text_wind);

        if (_show_cash) {
            window.draw(*_text_cash);

            window.draw(*_text_weapon);
            window.draw(*_text_weapon_count);
        }

        _draw_shop(window);
    }

    void Hud::update(sf::Time& delta, double wind)
    {
        _flag->set_speed(pmt::util::linear(abs(wind), 0.02, 0.4, -1, 1));
        _flag->set_scale(wind < 0 ? -2 : 2, 2);
        _flag->update(delta);

        _text_wind->setString(
            pmt::util::to_string_prec(
                fabs(pmt::config::WIND_MULTIPLIER * wind)) + " mph");
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

        _check_cash();
    }

    void Hud::shop_down()
    {
        if (_top_shop_item < pmt::offers.size() - 5)
            _top_shop_item++;

        if (_selected < pmt::offers.size() - 1)
            _selected++;

        _check_cash();
    }

    void Hud::open_shop(std::shared_ptr<pmt::Tank>& tank)
    {
        _top_shop_item = 0;
        _selected = 0;

        _shop_client = tank;
        _shop_open = true;

        _check_cash();
    }

    void Hud::close_shop()
    {
        _shop_open = false;
    }

    void Hud::buy()
    {
        auto& offer = pmt::offers[_selected];

        if (offer.price <= _shop_client->get_cash()) {
            _shop_client->buy(offer);
            close_shop();
        }
    }

    void Hud::show_turn(std::shared_ptr<pmt::Tank>& tank)
    {
        _text_turn_name->setString(tank->is_human() ? "Your turn" : "AI turn");

        double width = _text_turn_name->getLocalBounds().width;

        _text_turn_name->setPosition(pmt::config::WINDOW_W / 2.0 - 30 - width, 30);
    }

    void Hud::show_cash(std::shared_ptr<pmt::Tank>& tank)
    {
        _show_cash = tank->is_human() && tank->has_turn();

        if (! _show_cash) return;

        // Show weapon first
        _text_weapon->setString(tank->get_weapon_name());
        _text_weapon->setPosition(2, 12);

        _text_weapon_count->setPosition(_text_weapon->getLocalBounds().width + 8, 12);
        _text_weapon_count->setString("x" + std::to_string(tank->get_weapon_count()));

        // Cash second
        _text_cash->setString("$" + std::to_string(tank->get_cash()));
        _text_cash->setPosition(
            pmt::config::WINDOW_W - _text_cash->getLocalBounds().width - 2,
        12);
    }

    void Hud::_check_cash()
    {
        auto& offer = pmt::offers[_selected];

        _no_cash = offer.price > _shop_client->get_cash();

        if (_no_cash)
            _text_shop_info->setPosition(
                pmt::config::WINDOW_W / 2
                - _text_shop_info->getLocalBounds().width / 2,
                50
            );
    }

    void Hud::_draw_shop(sf::RenderWindow& window)
    {
        if (_shop_open) {
            window.draw(*_shop_overlay);
            window.draw(*_text_shop_header);

            if (_no_cash)
                window.draw(*_text_shop_info);

            _text_shop_header->setPosition(
                pmt::config::WINDOW_W - _text_shop_header->getLocalBounds().width,
                pmt::config::WINDOW_H - _text_shop_header->getLocalBounds().height + 32
            );

            int y = 80;

            sf::Color col;

            for (int i = 0; i < 5; i++) {
                auto& offer = offers[_top_shop_item + i];

                if (_selected == _top_shop_item + i) {
                    col = pmt::color::Blue;

                    if (offer.price > _shop_client->get_cash())
                        col = pmt::color::Red;
                } else {
                    col = sf::Color::White;
                }

                _shop_names[offer.type]->setColor(col);
                _shop_descs[offer.type]->setColor(col);
                _shop_prices[offer.type]->setColor(col);

                _shop_prices[offer.type]->setPosition(90, y + 8);
                _shop_names[offer.type]->setPosition(150, y);
                _shop_descs[offer.type]->setPosition(150, y + 20);

                window.draw(*_shop_prices[offer.type]);
                window.draw(*_shop_names[offer.type]);
                window.draw(*_shop_descs[offer.type]);

                y += 60;
            }
        }
    }
}

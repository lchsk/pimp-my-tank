#ifndef HUD_H
#define HUD_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "tank.h"

namespace pmt
{
    enum class OfferType {
        Missile,
        MagnumMissile,

        ShotPower,
        ShowShotAngle,
        Transparency,
        Crosshairs,
        Shield25,
        Shield50,
        Shield100,

        RedColor,
        BlueColor,
        PinkColor,
        NoColor
    };

    class Offer
    {
    public:
        Offer(OfferType type, std::string name, std::string desc, int price)
            : type(type),
            name(name),
            desc(desc),
            price(price) {}
        ~Offer()
        {
        }

        OfferType type;
        int price;
        std::string name;
        std::string desc;
    };

    static std::vector<Offer> offers = {
        Offer(OfferType::Missile, "Missile", "Basic tank missile", 50),
        Offer(OfferType::MagnumMissile, "Magnum Missile", "Magnum Missile", 50),

        Offer(OfferType::Crosshairs, "Crosshairs", "Magnum Missile", 50),
        Offer(OfferType::ShotPower, "Show Power", "Magnum Missile", 50),
        Offer(OfferType::ShowShotAngle, "Shot Angle", "Magnum Missile", 50),
        Offer(OfferType::Transparency, "Transparency", "Magnum Missile", 50),
        Offer(OfferType::Shield25, "Shield +25", "Magnum Missile", 50),
        Offer(OfferType::Shield50, "Shield +50", "Magnum Missile", 50),
        Offer(OfferType::Shield100, "SHield +100", "Magnum Missile", 50),

        Offer(OfferType::RedColor, "Red tank color", "Paint your tank red", 50),
        Offer(OfferType::BlueColor, "Blue tank color", "Paint your tank blue", 50),
        Offer(OfferType::PinkColor, "Pink tank color", "Paint your tank pink", 50),
        Offer(OfferType::NoColor, "No tank color", "Default tank color", 50),
    };

    class Hud
    {
    public:
        Hud(std::shared_ptr<sf::Font>& font);
        ~Hud();

        void render(sf::RenderWindow& window);
        void update(sf::Time& delta);

        void show_turn(std::shared_ptr<pmt::Tank>& tank);
        void show_cash(std::shared_ptr<pmt::Tank>& tank);

        bool is_shop_open() const;
        void open_shop(std::shared_ptr<pmt::Tank>& tank);
        void close_shop();
        void shop_up();
        void shop_down();

    private:
        bool _show_cash;
        bool _shop_open;
        unsigned _top_shop_item;

        std::shared_ptr<pmt::Tank> _shop_client;

        std::shared_ptr<sf::Font> _font;

        std::unique_ptr<sf::Text> _text_turn_name;

        std::unique_ptr<sf::Text> _text_cash_label;
        std::unique_ptr<sf::Text> _text_cash;

        std::unique_ptr<sf::Text> _text_weapon_label;
        std::unique_ptr<sf::Text> _text_weapon;

        std::unordered_map<OfferType, std::unique_ptr<sf::Text> > _shop_names;
        std::unordered_map<OfferType, std::unique_ptr<sf::Text> > _shop_descs;
        std::unordered_map<OfferType, std::unique_ptr<sf::Text> > _shop_prices;

    };
}

#endif

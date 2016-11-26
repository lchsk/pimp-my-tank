#ifndef UTIL_H
#define UTIL_H

#include <iomanip>
#include <random>

#include <SFML/Graphics.hpp>

namespace pmt
{
    enum class OfferType {
        Missile,
        MagnumMissile,

        ShotPower,
        ShowShotAngle,
        Crosshairs,
        Shield25,
        Shield50,
        Shield100,

		Transparency,
        RedColor,
        BlueColor,
        PinkColor,
        NoColor
    };

    enum class WeaponType {
        Missile,
        MagnumMissile
    };

    class Offer
    {
    public:
        Offer(OfferType type, std::string name, std::string desc, int price)
            : type(type),
            price(price),
            name(name),
            desc(desc) {}
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

        Offer(OfferType::Shield25, "Shield +25", "Magnum Missile", 50),
        Offer(OfferType::Shield50, "Shield +50", "Magnum Missile", 50),
        Offer(OfferType::Shield100, "SHield +100", "Magnum Missile", 50),

        Offer(OfferType::RedColor, "Red tank color", "Paint your tank red", 50),
        Offer(OfferType::BlueColor, "Blue tank color", "Paint your tank blue", 50),
        Offer(OfferType::PinkColor, "Pink tank color", "Paint your tank pink", 50),
        Offer(OfferType::Transparency, "Transparency", "Magnum Missile", 50),
        Offer(OfferType::NoColor, "No tank color", "Default tank color", 0),
    };

    namespace config
    {
        const unsigned WINDOW_W = 800;
        const unsigned WINDOW_H = 448;

        const double MISSILE_SPEED = 2.5f;

        const int REWARD_BEING_HIT = 50;
        const int REWARD_HIT = 50;

        const double WIND_POWER_COEF = 30.0;

        const double WIND_MULTIPLIER = 8.0;

        const double G = 20.0;
        const double GAMMA_WIND = 180.0;
        const double C_AIR = 3.0;
        const double C_WIND = 50.0;
    }

    namespace util
    {
        const double PI = 3.14159265359;

        inline double radian(double a)
        {
            return a / 180.0 * PI;
        }

        inline double linear(
            double x,
            double target_min,
            double target_max,
            double input_min = 0,
            double input_max = 100)
        {
            return (target_max - target_min) *
                (x - input_min) / (input_max - input_min) + target_min;
        }

        inline std::string to_string_prec(double val, unsigned precision=2)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << val;

            return stream.str();
        }

        inline double get_random(double min, double max)
        {
            std::uniform_real_distribution<double> urd(min, max);
            std::random_device rd;
            std::mt19937 mt(rd());

            return urd(mt);
        }
    }

    namespace color
    {
        const sf::Color Blue = sf::Color(31, 58, 147, 255);
        const sf::Color Pink = sf::Color(219, 10, 91, 255);
        const sf::Color Transparent = sf::Color(255, 255, 255, 100);
    }
}

#endif

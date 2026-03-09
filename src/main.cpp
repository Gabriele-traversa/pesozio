
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <optional>
#include <random>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 500}), "Bandiera LGBT con Fiamme - SFML 3");
    window.setFramerateLimit(60);

    const int stripeCount = 6;
    const int flameCount = 40;

    std::array<sf::Color, stripeCount> colors = {
        sf::Color(228, 3, 3),
        sf::Color(255, 140, 0),
        sf::Color(255, 237, 0),
        sf::Color(0, 128, 38),
        sf::Color(0, 77, 255),
        sf::Color(117, 7, 135)
    };

    std::array<sf::RectangleShape, stripeCount> stripes;

    std::vector<sf::ConvexShape> flames(flameCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> heightDist(60.f, 150.f);
    std::uniform_real_distribution<float> offsetDist(-20.f, 20.f);

    auto updateStripes = [&]()
    {
        float stripeHeight = static_cast<float>(window.getSize().y) / stripeCount;

        for (int i = 0; i < stripeCount; ++i)
        {
            stripes[i].setSize({static_cast<float>(window.getSize().x), stripeHeight});
            stripes[i].setPosition({0.f, i * stripeHeight});
            stripes[i].setFillColor(colors[i]);
        }
    };

    auto updateFlames = [&]()
    {
        float width = static_cast<float>(window.getSize().x);
        float height = static_cast<float>(window.getSize().y);

        float flameWidth = width / flameCount;

        for (int i = 0; i < flameCount; ++i)
        {
            float flameHeight = heightDist(gen);

            flames[i].setPointCount(3);
            flames[i].setPoint(0, {i * flameWidth, height});
            flames[i].setPoint(1, {i * flameWidth + flameWidth / 2.f + offsetDist(gen), height - flameHeight});
            flames[i].setPoint(2, {(i + 1) * flameWidth, height});

            flames[i].setFillColor(sf::Color(255, 100 + rand() % 155, 0, 200));
        }
    };

    updateStripes();

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::Resized>())
                updateStripes();
        }

        // Aggiorna fiamme ogni 0.08 secondi
        if (clock.getElapsedTime().asSeconds() > 0.08f)
        {
            updateFlames();
            clock.restart();
        }

        window.clear();

        for (const auto& stripe : stripes)
            window.draw(stripe);

        for (const auto& flame : flames)
            window.draw(flame);

        window.display();
    }

    return 0;
}
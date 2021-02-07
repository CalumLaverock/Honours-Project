#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>

#define PI 3.14159265
#define RAND0TO1 ((double)rand() / (RAND_MAX))

sf::Vector2f getRandomPointInCircle(float radius, sf::Vector2f centre);
std::vector<sf::RectangleShape> generateRooms(int numRooms, float radius, sf::Vector2f centre);

int main()
{
    srand(time(NULL));
    float radius = 200.f;
    sf::Vector2f centre(800.f, 450.f);

    std::vector<sf::RectangleShape> rooms;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(centre.x - radius, centre.y - radius);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    rooms.clear();
                    rooms = generateRooms(40, 100.f, centre);
                }
            }
        }

        window.clear();
        window.draw(shape);
        for (auto room : rooms)
        {
            window.draw(room);
        }
        window.display();
    }

    return 0;
}

sf::Vector2f getRandomPointInCircle(float radius, sf::Vector2f centre)
{
    float random = ((double)rand() / (RAND_MAX));

    float t = 2 * PI * RAND0TO1;
    float u = RAND0TO1 + RAND0TO1;
    float r = NULL;

    if (u > 1)
        r = 2 - u;
    else
        r = u;

    sf::Vector2f randPoint(radius * r * std::cos(t) + centre.x, radius * r * std::sin(t) + centre.y);

    return randPoint;
}

std::vector<sf::RectangleShape> generateRooms(int numRooms, float radius, sf::Vector2f centre)
{
    std::vector<sf::RectangleShape> rooms;

    for (int i = 0; i < numRooms; i++)
    {
        sf::RectangleShape room;
        sf::Vector2f roomSize((rand() % 120) + 80, (rand() % 80) + 20);

        room.setSize(roomSize);
        room.setFillColor(sf::Color::Green);
        room.setOutlineThickness(3.f);
        room.setOutlineColor(sf::Color::Blue);

        room.setPosition(getRandomPointInCircle(radius, centre) - (roomSize / 2.f));

        rooms.push_back(room);
    }

    return rooms;
}
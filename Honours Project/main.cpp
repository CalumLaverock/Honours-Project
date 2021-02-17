#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include "Room.h"

#define PI 3.14159265
#define RAND0TO1 ((double)rand() / (RAND_MAX))

sf::Vector2f getRandomPointInCircle(float radius, sf::Vector2f centre);
void generateRooms(int numRooms, float radius, sf::Vector2f centre, std::vector<Room*>& rooms);

int main()
{
    srand(time(NULL));
    float radius = 200.f;
    sf::Vector2f centre(800.f, 450.f);

    std::vector<Room*> rooms;

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
                    for (auto room : rooms)
                    {
                        delete room;
                    }
                    rooms.clear();

                    generateRooms(60, 100.f, centre, rooms);
                }

                if (event.key.code == sf::Keyboard::S)
                {
                    for (auto room : rooms)
                    {
                        room->CheckCollisions(rooms);
                    }
                }
            }
        }

        window.clear();
        window.draw(shape);
        for (auto room : rooms)
        {
            window.draw(room->getShape());
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

void generateRooms(int numRooms, float radius, sf::Vector2f centre, std::vector<Room*>& rooms)
{
    for (int i = 0; i < numRooms; i++)
    {
        Room* room = new Room();

        sf::RectangleShape roomShape;
        sf::Vector2f roomSize((rand() % 120) + 80, (rand() % 80) + 20);

        roomShape.setSize(roomSize);
        roomShape.setFillColor(sf::Color::Green);
        roomShape.setOutlineThickness(3.f);
        roomShape.setOutlineColor(sf::Color::Blue);
        
        roomShape.setOrigin(roomSize / 2.f);
        roomShape.setPosition(getRandomPointInCircle(radius, centre));

        room->setShape(roomShape);
        rooms.push_back(room);
    }
}
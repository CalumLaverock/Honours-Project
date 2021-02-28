#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include "RoomManager.h"

int main()
{
    srand(time(NULL));
    float radius = 200.f;
    sf::Vector2f centre(800.f, 450.f);

    RoomManager* roomManager = new RoomManager();
    roomManager->Init(radius / 2, centre);

    sf::RenderWindow window(sf::VideoMode(1600, 900), "Competitive FPS Map Generation");
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(centre.x - radius, centre.y - radius);

    bool collide = false;
    bool onlySelected = false;

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
                    collide = false;

                    roomManager->ClearRooms();

                    roomManager->GenerateRooms(75);
                    roomManager->SelectRoomsBySize(75.f, 75.f);
                }

                if (event.key.code == sf::Keyboard::S)
                {
                    collide = true;
                }

                if (event.key.code == sf::Keyboard::T)
                {
                    onlySelected = !onlySelected;
                }
            }
        }

        // separate rooms once S key is pressed
        if (collide)
        {
            roomManager->SeparateRooms(collide);
        }

        // change title while rooms are separating
        std::string title = collide ? "Separating Rooms..." : "Competitive FPS Map Generation";
        window.setTitle(title);

        // render the rooms
        window.clear();
        window.draw(shape);
        for (auto room : roomManager->GetRooms())
        {
            // allow for toggling of only drawing selected rooms
            if (onlySelected)
            {
                if (room->IsSelected())
                {
                    window.draw(room->getShape());
                }
            }
            else
            {
                window.draw(room->getShape());
            }
        }
        window.display();
    }

    return 0;
}
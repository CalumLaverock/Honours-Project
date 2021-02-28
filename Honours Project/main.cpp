#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include "RoomManager.h"

int main()
{
    //---INIT---
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Competitive FPS Map Generation");

    srand(time(NULL));
    float radius = 200.f;
    sf::Vector2f centre(800.f, 450.f);

    RoomManager* roomManager = new RoomManager();
    roomManager->Init(radius / 2, centre);

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

            //---HANDLE INPUT---
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    // stop room separation, generate a new set of rooms, highlight rooms above a certain size
                    collide = false;

                    roomManager->ClearRooms();

                    roomManager->GenerateRooms(70);
                    roomManager->SelectRoomsBySize(75.f, 75.f);
                }

                if (event.key.code == sf::Keyboard::S)
                {
                    // start room separation
                    collide = true;
                }

                if (event.key.code == sf::Keyboard::T)
                {
                    // toggle only drawing the selected rooms
                    onlySelected = !onlySelected;
                }

                if (event.key.code == sf::Keyboard::F)
                {
                    // select random already selected room
                    // select another random already selected room that is a certain distance from the first room
                    roomManager->SelectObjectiveRooms();
                }

                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        //---UPDATE---
        // separate rooms once S key is pressed
        if (collide)
        {
            roomManager->SeparateRooms(collide);
        }

        // change title while rooms are separating
        std::string title = collide ? "Separating Rooms..." : "Competitive FPS Map Generation";
        window.setTitle(title);

        //---RENDER---
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
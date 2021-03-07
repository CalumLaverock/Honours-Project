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
    bool separated = false;
    bool drawSelected = false;
    bool drawBounds = false;
    bool debugMode = false;

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
                if (!debugMode)
                {
                    // outwith debug mode, allow the user to generate the entire map with one key
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        // stop room separation, generate a new set of rooms, and start room separation
                        collide = false;

                        roomManager->ClearRooms();

                        roomManager->GenerateRooms(70);

                        collide = true;
                    }
                }
                else
                {
                    // within debug mode, allow the user to perform each step of map generation separately
                    if (event.key.code == sf::Keyboard::G)
                    {
                        // stop room separation and generate a new set of rooms
                        collide = false;
                        separated = false;

                        roomManager->ClearRooms();

                        roomManager->GenerateRooms(70);
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        // start room separation
                        collide = true;
                    }

                    if (event.key.code == sf::Keyboard::R)
                    {
                        // select all rooms above certain x and y lengths
                        roomManager->SelectRoomsBySizeAndConnections(75.f, 75.f);
                    }

                    if (event.key.code == sf::Keyboard::O)
                    {
                        if (separated)
                        {
                            // select the objective rooms
                            roomManager->SelectObjectiveRooms();
                        }
                    }

                    if (event.key.code == sf::Keyboard::N)
                    {
                        // toggle only drawing the selected rooms
                        drawSelected = !drawSelected;
                    }

                    if (event.key.code == sf::Keyboard::B)
                    {
                        // toggle the drawing of the objective room selection boundaries
                        drawBounds = !drawBounds;
                    }
                }

                if (event.key.code == sf::Keyboard::D)
                {
                    // toggle debug mode
                    debugMode = !debugMode;
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
            
            if (!collide)
            {
                if (!debugMode)
                {
                    // while not in debug mode, automatically select the objective rooms when the rooms have finished separating
                    roomManager->SelectRoomsBySizeAndConnections(75.f, 75.f);
                    roomManager->SelectObjectiveRooms();
                }
                else
                {
                    // while in debug mode, set a flag when the rooms have finished separating
                    separated = true;
                }
            }
        }

        // change title while rooms are separating
        std::string title = collide ? "Separating Rooms..." : "Competitive FPS Map Generation";
        window.setTitle(title);

        //---RENDER---
        window.clear();

        window.draw(shape);

        for (auto room : roomManager->getRooms())
        {
            // allow for toggling of only drawing selected rooms
            if (drawSelected)
            {
                if (room->isSelected())
                {
                    window.draw(room->getShape());
                }
            }
            else
            {
                window.draw(room->getShape());
            }
        }

        if (drawBounds)
        {
            // draw the bounding rectangles around the first objective room
            window.draw(roomManager->getOuterBound());
            window.draw(roomManager->getInnerBound());
        }

        window.display();
    }

    return 0;
}
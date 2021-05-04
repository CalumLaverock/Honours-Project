#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include "RoomManager.h"
#include <iostream>

int main()
{
    //---INIT---
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Competitive FPS Map Generation");

    srand(time(NULL));
    float radius = 200.f;
    sf::Vector2f centre(970.f, 450.f);

    sf::Font font;
    if (!font.loadFromFile("fonts/coolvetica.ttf"))
    {
        std::cout << "ERROR: Failed to load font" << std::endl;
    }

    RoomManager* roomManager = new RoomManager();
    roomManager->Init(radius / 2, centre);

    bool restart = false;
    bool collide = false;
    bool separated = false;
    bool connected = false;
    bool drawSelected = false;
    bool drawBounds = false;
    bool debugMode = false;

    //---UPDATE---
    while (window.isOpen())
    {
        // if the restart flag is set, start room generation from the beginning
        if (restart)
        {
            // unset the restart flag, stop room separation, generate a new set of rooms, and start room separation 
            restart = false;
            collide = false;
            separated = false;
            connected = false;

            roomManager->ClearRooms();

            roomManager->GenerateRooms(70);

            collide = true;
        }

        //---HANDLE INPUT---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (!debugMode)
                {
                    // outwith debug mode, allow the user to generate the entire map with one key
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        // unset the restart flag, stop room separation, generate a new set of rooms, and start room separation 
                        restart = false;
                        collide = false;
                        separated = false;
                        connected = false;

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
                        restart = false;
                        collide = false;
                        separated = false;
                        connected = false;

                        roomManager->ClearRooms();

                        roomManager->GenerateRooms(70);
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        // start room separation
                        collide = true;
                    }

                    if (separated)
                    {
                        if (event.key.code == sf::Keyboard::R)
                        {
                            // rooms must be separated before selection as number of connected rooms can only be properly calculated
                            // after separation
                            // select all rooms above certain x and y lengths
                            roomManager->SelectRoomsBySizeAndConnections(45.f, 45.f, 3);
                        }

                        if (event.key.code == sf::Keyboard::O)
                        {
                            // select the objective rooms
                            if (!roomManager->SelectObjectiveRooms())
                            {
                                restart = true;
                            }
                        }

                        if (event.key.code == sf::Keyboard::A)
                        {
                            // rooms must be separated before selection as number of connected rooms can only be properly calculated
                            // after separation
                            // select all rooms above certain x and y lengths
                            roomManager->SelectRoomsBySizeAndConnections(45.f, 45.f, 2);
                        }

                        if (event.key.code == sf::Keyboard::P)
                        {
                            // select the spawn rooms
                            if (!roomManager->SelectSpawnRooms())
                            {
                                restart = true;
                            }
                        }

                        if (event.key.code == sf::Keyboard::C)
                        {
                            // only allow the connection process once per map, running ConnectRooms more than once in a map will delete all rooms
                            if (!connected)
                            {
                                // connect each spawn room to the other important rooms and remove all non-connected rooms
                                roomManager->ConnectRooms();

                                connected = true;
                            }
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

        // separate rooms once S key is pressed
        if (collide)
        {
            roomManager->SeparateRooms(collide);
            
            if (!collide)
            {
                if (!debugMode)
                {
                    // while not in debug mode, automatically select the objective rooms when the rooms have finished separating
                    roomManager->SelectRoomsBySizeAndConnections(45.f, 45.f, 3);
                    if (!roomManager->SelectObjectiveRooms())
                    {
                        restart = true;
                    }
                    roomManager->SelectRoomsBySizeAndConnections(45.f, 45.f, 2);
                    if (!roomManager->SelectSpawnRooms())
                    {
                        restart = true;
                    }
                    if (!restart)
                    {
                        roomManager->ConnectRooms();
                        connected = true;
                    }
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

        if (roomManager->getRooms().size() > 0)
        {
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
        }
        else
        {
            for (auto finalRoom : roomManager->getFinalRooms())
            {
                window.draw(finalRoom->getShape());
            }
        }

        if (connected)
        {
            for (auto bounds : roomManager->getConnectionBounds())
            {
                // set the bounding rectangles to cyan to match the other rooms in the map
                bounds.setFillColor(sf::Color::Cyan);

                window.draw(bounds);
            }

            for (auto obj : roomManager->getObjectiveRooms())
            {
                window.draw(obj->getShape());
            }
            window.draw(roomManager->getAtkSpawn()->getShape());
            window.draw(roomManager->getDefSpawn()->getShape());
        }

        if (drawBounds)
        {
            if (connected)
            {
                for (auto bounds : roomManager->getConnectionBounds())
                {
                    // set the bounding rectangles to transluscent red to stand out
                    bounds.setFillColor(sf::Color(0xff, 0x00, 0x00, 0xff));

                    window.draw(bounds);
                }
            }
        }

        //---DRAW HUD---
        sf::Text debugText("[D]ebug Mode:", font, 45), debugToggle;
        debugText.setPosition(0, 0);

        std::string debugOnOff = debugMode ? "On" : "Off";
        sf::Color debugColor = debugMode ? sf::Color::Green : sf::Color::Red;
        debugToggle.setString(debugOnOff);
        debugToggle.setFillColor(debugColor);
        debugToggle.setFont(font);
        debugToggle.setCharacterSize(45);
        debugToggle.setPosition(280, 0);

        window.draw(debugToggle);
        window.draw(debugText);

        window.display();
    }

    return 0;
}
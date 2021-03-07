#include "RoomManager.h"

RoomManager::RoomManager()
{
    circleRad = 0.f;
    circleCentre.x = 0.f;
    circleCentre.y = 0.f;
}

void RoomManager::Init(float radius, sf::Vector2f centre)
{
    circleRad = radius;
    circleCentre = centre;
}

void RoomManager::GenerateRooms(int numRooms)
{
    for (int i = 0; i < numRooms; i++)
    {
        Room* room = new Room();

        sf::RectangleShape roomShape;

        sf::Vector2f roomSize;

        //random room size between 20 and 140
        roomSize.x = (rand() % 120) + 20;
        roomSize.y = (rand() % 120) + 20;

        roomShape.setSize(roomSize);
        roomShape.setOrigin(roomSize / 2.f);
        roomShape.setPosition(GetRandomPointInCircle());

        roomShape.setFillColor(sf::Color::Green);
        roomShape.setOutlineThickness(3.f);
        roomShape.setOutlineColor(sf::Color::Blue);

        room->setShape(roomShape);
        rooms.push_back(room);
    }
}

void RoomManager::SeparateRooms(bool& collide)
{
    collide = false;

    for (auto room : rooms)
    {
        // set collide to true when one room returns true and do not set to false afterwards
        bool localCollide = false;
        localCollide = room->CheckCollisions(rooms);

        if (localCollide)
        {
            collide = true;
        }
    }
}

void RoomManager::SelectRoomsBySizeAndConnections(float xThreshold, float yThreshold)
{
    for (auto room : rooms)
    {
        room->CalculateConnectedRooms(rooms);

        if ((room->getShape().getSize().x >= xThreshold && room->getShape().getSize().y >= yThreshold) &&
            (room->getConnections() >= 3))
        {
            room->ChangeColour(sf::Color::Cyan);

            room->setSelected(true);
        }
    }
}

void RoomManager::SelectObjectiveRooms()
{
    finalRooms.clear();

    bool selectingRooms = true;

    Room* firstObjRoom = NULL;
    Room* secondObjRoom = NULL;

    while (selectingRooms)
    {
        selectingRooms = false;

        firstObjRoom = rooms[rand() % rooms.size()];
        while (!firstObjRoom->isSelected())
        {
            firstObjRoom = rooms[rand() % rooms.size()];
        }

        // create an inner bound rectangle which will prevent rooms from being selected as objective rooms within it
        // this inner bound is 3 times the size of the first room
        sf::Vector2f innerBoundSize(firstObjRoom->getShape().getSize() * 3.f);
        innerBound.setSize(innerBoundSize);
        innerBound.setOrigin(innerBoundSize / 2.f);
        innerBound.setPosition(firstObjRoom->getShape().getPosition());

        sf::Color innerBoundColor(0xff, 0xff, 0xff, 0x77);
        innerBound.setFillColor(innerBoundColor);

        // create an outer bound rectangle which will mean only rooms within it can be selected as objective rooms
        // this outer bound is 6 times the size of the first room
        sf::Vector2f outerBoundSize(firstObjRoom->getShape().getSize() * 6.f);
        outerBound.setSize(outerBoundSize);
        outerBound.setOrigin(outerBoundSize / 2.f);
        outerBound.setPosition(firstObjRoom->getShape().getPosition());

        sf::Color outerBoundColor(0xff, 0x77, 0xff, 0x77);
        outerBound.setFillColor(outerBoundColor);

        int timeout = 0;

        secondObjRoom = rooms[rand() % rooms.size()];
        // while the room is in the inner bounds, or not in the outer bounds, or is not a selected room
        // get a new room and test it against these parameters again
        while (secondObjRoom->getShape().getGlobalBounds().intersects(innerBound.getGlobalBounds())
            || !secondObjRoom->getShape().getGlobalBounds().intersects(outerBound.getGlobalBounds())
            || !secondObjRoom->isSelected())
        {
            secondObjRoom = rooms[rand() % rooms.size()];

            // only try an arbitrary number of rooms, if none fit the criteria then start the entire room selection process again
            timeout += 1;
            if (timeout >= 19)
            {
                selectingRooms = true;
                break;
            }
        }
    }

    // colour the objective rooms magenta and push them onto a separate vector
    firstObjRoom->ChangeColour(sf::Color::Magenta);
    secondObjRoom->ChangeColour(sf::Color::Magenta);

    finalRooms.push_back(firstObjRoom);
    finalRooms.push_back(secondObjRoom);
}

// random point in circle function taken from [--INSERT WEBPAGE--]
sf::Vector2f RoomManager::GetRandomPointInCircle()
{
    float t = 2 * 3.14159 * ((double)rand() / (RAND_MAX));
    float u = ((double)rand() / (RAND_MAX)) + ((double)rand() / (RAND_MAX));
    float r = NULL;

    if (u > 1)
        r = 2 - u;
    else
        r = u;

    sf::Vector2f randPoint(circleRad * r * std::cos(t) + circleCentre.x, circleRad * r * std::sin(t) + circleCentre.y);

    return randPoint;
}

void RoomManager::ClearRooms()
{
    for (auto room : rooms)
    {
        delete room;
    }
    rooms.clear();
}
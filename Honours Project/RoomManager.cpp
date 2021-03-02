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
        roomShape.setFillColor(sf::Color::Green);
        roomShape.setOutlineThickness(3.f);
        roomShape.setOutlineColor(sf::Color::Blue);

        roomShape.setOrigin(roomSize / 2.f);
        roomShape.setPosition(GetRandomPointInCircle());

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

void RoomManager::SelectRoomsBySize(float xThreshold, float yThreshold)
{
    for (auto room : rooms)
    {
        if (room->getShape().getSize().x >= xThreshold && room->getShape().getSize().y >= yThreshold)
        {
            sf::RectangleShape newShape;
            sf::RectangleShape oldShape = room->getShape();

            // change the room's fill color to cyan if it is selected
            newShape = oldShape;
            newShape.setFillColor(sf::Color::Cyan);

            room->SetSelected(true);

            room->setShape(newShape);
        }
    }
}

void RoomManager::SelectObjectiveRooms()
{
    selectedRooms.clear();

    bool selectingRooms = true;
    Room* firstObjRoom = NULL;
    Room* secondObjRoom = NULL;

    while (selectingRooms)
    {
        selectingRooms = false;

        firstObjRoom = rooms[rand() % rooms.size()];
        while (!firstObjRoom->IsSelected())
        {
            firstObjRoom = rooms[rand() % rooms.size()];
        }

        sf::Vector2f innerBoundSize(firstObjRoom->getShape().getSize() * 3.f);
        innerBound.setSize(innerBoundSize);
        innerBound.setOrigin(innerBoundSize / 2.f);
        innerBound.setPosition(firstObjRoom->getShape().getPosition());

        sf::Color innerBoundColor(0xff, 0xff, 0xff, 0x99);
        innerBound.setFillColor(innerBoundColor);

        sf::Vector2f outerBoundSize(firstObjRoom->getShape().getSize() * 5.f);
        outerBound.setSize(outerBoundSize);
        outerBound.setOrigin(outerBoundSize / 2.f);
        outerBound.setPosition(firstObjRoom->getShape().getPosition());

        sf::Color outerBoundColor(0xff, 0x77, 0xff, 0x99);
        outerBound.setFillColor(outerBoundColor);

        int timeout = 0;

        secondObjRoom = rooms[rand() % rooms.size()];
        // while the room is in the inner bounds, not in the outer bounds, or is not a selected room
        // get a new room and test it against these parameters again
        while (secondObjRoom->getShape().getGlobalBounds().intersects(innerBound.getGlobalBounds())
            || !secondObjRoom->getShape().getGlobalBounds().intersects(outerBound.getGlobalBounds())
            || !secondObjRoom->IsSelected())
        {
            secondObjRoom = rooms[rand() % rooms.size()];

            // only try 20 rooms, otherwise start the room selection over again
            timeout += 1;
            if (timeout > 20)
            {
                selectingRooms = true;
                break;
            }
        }
    }

    // deselect all rooms that aren't the objective rooms and change their colour back to green
    for (auto room : rooms)
    {
        if (room != firstObjRoom && room != secondObjRoom)
        {
            sf::RectangleShape newShape;
            sf::RectangleShape oldShape = room->getShape();

            // change the room's fill color to cyan if it is selected
            newShape = oldShape;
            newShape.setFillColor(sf::Color::Green);

            room->SetSelected(false);

            room->setShape(newShape);
        }
        else
        {
            sf::RectangleShape newShape;
            sf::RectangleShape oldShape = room->getShape();

            // change the room's fill color to cyan if it is selected
            newShape = oldShape;
            newShape.setFillColor(sf::Color::Cyan);

            room->setShape(newShape);

            selectedRooms.push_back(room);
        }
    }
}

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
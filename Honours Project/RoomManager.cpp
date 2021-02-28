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
   // selectedRooms.clear();

    int random = rand() % rooms.size();
    Room* firstObjRoom = rooms[random];
    while (!firstObjRoom->IsSelected())
    {
        random = rand() % rooms.size();
        firstObjRoom = rooms[random];
    }

    random = rand() % rooms.size();
    Room* secondObjRoom = rooms[random];
    while ((secondObjRoom->getShape().getPosition().x > (firstObjRoom->getShape().getPosition().x - firstObjRoom->getShape().getSize().x * 2)
        && secondObjRoom->getShape().getPosition().x < (firstObjRoom->getShape().getPosition().x + firstObjRoom->getShape().getSize().x * 2)
        && secondObjRoom->getShape().getPosition().y > (firstObjRoom->getShape().getPosition().y - firstObjRoom->getShape().getSize().y * 2)
        && secondObjRoom->getShape().getPosition().y < (firstObjRoom->getShape().getPosition().y + firstObjRoom->getShape().getSize().y * 2))
        || !secondObjRoom->IsSelected())
    {
        random = rand() % rooms.size();
        secondObjRoom = rooms[random];
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
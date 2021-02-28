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

            // change the room's fill color to magenta if it is selected
            newShape = oldShape;
            newShape.setFillColor(sf::Color::White);

            room->SetSelected(true);

            room->setShape(newShape);
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
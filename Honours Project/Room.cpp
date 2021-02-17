#include "Room.h"

void Room::CheckCollisions(std::vector<Room*> rooms)
{
	movement.x = 0.f; 
	movement.y = 0.f;

	for (Room* currentRoom : rooms)
	{
		if (currentRoom != this)
		{
			// check if this room intersects with any other room
			if (shape.getGlobalBounds().intersects(currentRoom->getShape().getGlobalBounds()))
			{
				// check where currentRoom is in relation to this room
				// add 1.f or -1.f to movement vector based on relative position
				sf::Vector2f offset = shape.getPosition() - currentRoom->getShape().getPosition();
				movement.x += offset.x / std::abs(offset.x);
				movement.y += offset.y / std::abs(offset.y);
			}
		}
	}

	shape.move(movement);
}
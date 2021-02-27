#include "Room.h"

bool Room::CheckCollisions(std::vector<Room*> rooms)
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
				float magnitude = std::abs(std::sqrt(std::pow(offset.x, 2) + std::pow(offset.y, 2)));

				movement.x += offset.x / magnitude;
				movement.y += offset.y / magnitude;
			}
		}
	}

	shape.move(movement);

	// return true if the room has to move this frame
	return (movement.x > 0.f || movement.y > 0.f);
}
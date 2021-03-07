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

void Room::ChangeColour(sf::Color newColour)
{
	sf::RectangleShape newShape;
	sf::RectangleShape oldShape = shape;

	// change the room's fill color to cyan if it is selected
	newShape = oldShape;
	newShape.setFillColor(newColour);

	shape = newShape;
}

void Room::CalculateConnectedRooms(std::vector<Room*> rooms)
{
	connectedRooms = 0;

	sf::RectangleShape boundingBox;
	sf::Vector2f size(shape.getSize().x + 10.f, shape.getSize().y + 10.f);
	boundingBox.setSize(size);
	boundingBox.setOrigin(size / 2.f);
	boundingBox.setPosition(shape.getPosition());

	for (auto room : rooms)
	{
		if (room != this)
		{
			if (boundingBox.getGlobalBounds().intersects(room->getShape().getGlobalBounds()))
			{
				connectedRooms++;
			}

			// if required to check direction, check connected rooms position relative to this room
		}
	}
}
#pragma once
#include <SFML/Graphics.hpp>

class Room
{
public:
	Room() { movement.x = 0.f; movement.y = 0.f; }

	sf::RectangleShape getShape() { return shape; }
	void setShape(sf::RectangleShape newShape) { shape = newShape; }

	void CheckCollisions(std::vector<Room*> rooms);

private:
	sf::RectangleShape shape;
	sf::Vector2f movement;
};


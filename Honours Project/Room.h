#pragma once
#include <SFML/Graphics.hpp>

class Room
{
public:
	Room() { movement.x = 0.f; movement.y = 0.f; selected = false; }

	sf::RectangleShape getShape() { return shape; }
	void setShape(sf::RectangleShape newShape) { shape = newShape; }

	bool CheckCollisions(std::vector<Room*> rooms);

	bool IsSelected() { return selected; }
	void SetSelected(bool select) { selected = select; }

private:
	sf::RectangleShape shape;
	sf::Vector2f movement;
	bool selected;
};


#pragma once
#include <SFML/Graphics.hpp>

class Room
{
public:
	Room() : movement(0.f,0.f), selected(false), lockedIn(false), connectedRooms(0) { }
	bool CheckCollisions(std::vector<Room*> rooms);
	void ChangeColour(sf::Color newColour);
	void CalculateConnectedRooms(std::vector<Room*> rooms);

	sf::RectangleShape getShape() { return shape; }
	void setShape(sf::RectangleShape newShape) { shape = newShape; }
	bool isSelected() { return selected; }
	void setSelected(bool select) { selected = select; }
	bool isLockedIn() { return lockedIn; }
	void setLockedIn(bool locked) { lockedIn = locked; }
	int getConnections() { return connectedRooms; }

private:
	sf::RectangleShape shape;
	sf::Vector2f movement;
	bool selected;
	bool lockedIn;
	int connectedRooms;
};


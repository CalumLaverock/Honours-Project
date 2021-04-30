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
	sf::RectangleShape getInnerBound() { return innerBound; }
	void setInnerBound(sf::RectangleShape bound) { innerBound = bound; }
	sf::RectangleShape getOuterBound() { return outerBound; }
	void setOuterBound(sf::RectangleShape bound) { outerBound = bound; }
	bool isSelected() { return selected; }
	void setSelected(bool select) { selected = select; }
	bool isLockedIn() { return lockedIn; }
	void setLockedIn(bool locked) { lockedIn = locked; }

	int getConnections() { return connectedRooms; }

private:
	sf::RectangleShape shape;
	sf::RectangleShape innerBound;
	sf::RectangleShape outerBound;

	sf::Vector2f movement;
	bool selected;
	bool lockedIn;
	int connectedRooms;
};


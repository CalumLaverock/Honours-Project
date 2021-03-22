#pragma once
#include "Room.h"

class RoomManager
{
public:
	RoomManager();

	void Init(float radius, sf::Vector2f centre);
	void GenerateRooms(int numRooms);
	void SeparateRooms(bool& collide);

	void SelectRoomsBySizeAndConnections(float xThreshold, float yThreshold, int numConnect = 0);
	void SelectObjectiveRooms();

	void ClearRooms();

	std::vector<Room*> getRooms() { return rooms; }
	sf::RectangleShape getInnerBound() { return innerBound; }
	sf::RectangleShape getOuterBound() { return outerBound; }

private:
	void UpdateSelectedRooms();
	sf::Vector2f GetRandomPointInCircle();

	float circleRad;
	sf::Vector2f circleCentre;
	std::vector<Room*> rooms;
	std::vector<Room*> selectedRooms;
	std::vector<Room*> finalRooms;

	sf::RectangleShape innerBound;
	sf::RectangleShape outerBound;
};


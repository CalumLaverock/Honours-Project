#pragma once
#include "Room.h"

class RoomManager
{
public:
	RoomManager();

	void Init(float radius, sf::Vector2f centre);
	void GenerateRooms(int numRooms);
	void SeparateRooms(bool& collide);
	void SelectRoomsBySize(float x, float y);

	std::vector<Room*> GetRooms() { return rooms; }
	void ClearRooms();

private:
	sf::Vector2f GetRandomPointInCircle();

	float circleRad;
	sf::Vector2f circleCentre;
	std::vector<Room*> rooms;
};

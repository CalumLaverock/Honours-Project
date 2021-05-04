#pragma once
#include "Room.h"

class RoomManager
{
public:
	RoomManager() : circleRad(0.f), circleCentre(0.f, 0.f), defSpawnRoom(nullptr), atkSpawnRoom(nullptr), restart(false) { }

	void Init(float radius, sf::Vector2f centre);
	void GenerateRooms(int numRooms);
	void SeparateRooms(bool& collide);

	void SelectRoomsBySizeAndConnections(sf::Vector2f sizeThreshold, int numConnect = 0);
	bool SelectObjectiveRooms();
	bool SelectSpawnRooms();
	void ConnectRooms();

	void ClearRooms();

	std::vector<Room*> getRooms() { return rooms; }
	std::vector<Room*> getSelectedRooms() { return selectedRooms; }
	std::vector<Room*> getFinalRooms() { return finalRooms; }
	std::vector<Room*> getObjectiveRooms() { return objectiveRooms; }
	Room* getAtkSpawn() { return atkSpawnRoom; }
	Room* getDefSpawn() { return defSpawnRoom; }
	std::vector<sf::RectangleShape> getConnectionBounds() { return connectionBoundingRectangles; }

private:
	void UpdateSelectedRooms();
	bool CheckSpawnConditions(bool defSpawn);
	std::vector<sf::Vector2f> CalculateVectors(Room* spawnRoom);
	sf::Vector2f GetRandomPointInCircle();
	float GetDistanceBetweenRooms(Room* r1, Room* r2);
	sf::Vector2f CalculateMidpoint(sf::Vector2f a, sf::Vector2f b);

	bool restart;

	float circleRad;
	sf::Vector2f circleCentre;

	Room* defSpawnRoom;
	Room* atkSpawnRoom;
	std::vector<Room*> rooms;
	std::vector<Room*> selectedRooms;
	std::vector<Room*> objectiveRooms;
	std::vector<Room*> finalRooms;

	std::vector<sf::RectangleShape> connectionBoundingRectangles;
};


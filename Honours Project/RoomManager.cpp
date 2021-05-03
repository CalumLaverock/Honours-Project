#include "RoomManager.h"
#include <math.h>

void RoomManager::Init(float radius, sf::Vector2f centre)
{
    circleRad = radius;
    circleCentre = centre;
}

void RoomManager::GenerateRooms(int numRooms)
{
    for (int i = 0; i < numRooms; i++)
    {
        Room* room = new Room();

        sf::RectangleShape roomShape;

        sf::Vector2f roomSize;

        //random room size between 20 and 140 units
        roomSize.x = (rand() % 120) + 20;
        roomSize.y = (rand() % 120) + 20;

        roomShape.setSize(roomSize);
        roomShape.setOrigin(roomSize / 2.f);
        roomShape.setPosition(GetRandomPointInCircle());

        roomShape.setFillColor(sf::Color::Green);

        room->setShape(roomShape);
        rooms.push_back(room);
    }

    restart = false;
}

void RoomManager::SeparateRooms(bool& collide)
{
    collide = false;

    for (auto room : rooms)
    {
        // set collide to true when one room returns true and do not set to false afterwards
        bool localCollide = false;
        localCollide = room->CheckCollisions(rooms);

        if (localCollide)
        {
            collide = true;
        }
    }
}

void RoomManager::SelectRoomsBySizeAndConnections(float xThreshold, float yThreshold, int numConnect)
{
    for (auto room : rooms)
    {
        room->setSelected(false);

        if (room->getShape().getSize().x >= xThreshold && room->getShape().getSize().y >= yThreshold)
        {
            // only calculate the number of connections if the room is the right size
            room->CalculateConnectedRooms(rooms);

            if (room->getConnections() >= numConnect)
            {
                room->setSelected(true);
            }
        }

        // if the room has been deselected but is locked in, then unlock it
        // if the room is still selected and is locked in, keep it locked in
        bool stillLocked = room->isLockedIn() && room->isSelected();
        room->setLockedIn(stillLocked);
    }

    UpdateSelectedRooms();
}

bool RoomManager::SelectObjectiveRooms()
{
    // remove current objective rooms to allow for reselecting objective rooms
    for (auto objectiveRoom : objectiveRooms)
    {
        objectiveRoom->setLockedIn(false);
    }
    objectiveRooms.clear();

    Room* firstObjRoom = nullptr;
    Room* secondObjRoom = nullptr;

    int restartTimeout = 0;

    bool selectingRooms = true;

    while (selectingRooms)
    {
        firstObjRoom = nullptr;
        secondObjRoom = nullptr;

        selectingRooms = false;

        restartTimeout += 1;
        if (restartTimeout > 5 || selectedRooms.size() <= 0)
        {
            restart = true;
            break;
        }

        firstObjRoom = selectedRooms[rand() % selectedRooms.size()];

        // create an inner bound rectangle which will prevent rooms from being selected as objective rooms within it
        // this inner bound is 3 times the size of the first room
        sf::RectangleShape innerBound;
        sf::Vector2f innerBoundSize(firstObjRoom->getShape().getSize() * 3.f);
        innerBound.setSize(innerBoundSize);
        innerBound.setOrigin(innerBoundSize / 2.f);
        innerBound.setPosition(firstObjRoom->getShape().getPosition());
        innerBound.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x37));

        firstObjRoom->setInnerBound(innerBound);

        // create an outer bound rectangle which will mean only rooms within it can be selected as objective rooms
        // this outer bound is 6 times the size of the first room
        sf::RectangleShape outerBound;
        sf::Vector2f outerBoundSize(firstObjRoom->getShape().getSize() * 6.f);
        outerBound.setSize(outerBoundSize);
        outerBound.setOrigin(outerBoundSize / 2.f);
        outerBound.setPosition(firstObjRoom->getShape().getPosition());
        outerBound.setFillColor(sf::Color(0xff, 0x77, 0xff, 0x37));

        firstObjRoom->setOuterBound(outerBound);

        int timeout = 0;

        secondObjRoom = selectedRooms[rand() % selectedRooms.size()];
        // while the room is in the inner bounds, or not in the outer bounds, or is not a selected room
        // get a new room and test it against these parameters again
        while (secondObjRoom->getShape().getGlobalBounds().intersects(firstObjRoom->getInnerBound().getGlobalBounds())
            || !secondObjRoom->getShape().getGlobalBounds().intersects(firstObjRoom->getOuterBound().getGlobalBounds()))
        {
            secondObjRoom = selectedRooms[rand() % selectedRooms.size()];

            // only try an arbitrary number of rooms, if none fit the criteria then start the entire room selection process again
            timeout += 1;
            if (timeout >= 19)
            {
                selectingRooms = true;
                break;
            }
        }
    }

    if (!restart)
    {
        // push the objective rooms onto a separate vector and set the relevant flags
        firstObjRoom->setLockedIn(true);
        secondObjRoom->setLockedIn(true);

        objectiveRooms.push_back(firstObjRoom);
        objectiveRooms.push_back(secondObjRoom);
    }
       
    UpdateSelectedRooms();

    return !restart;
}

bool RoomManager::SelectSpawnRooms()
{
    // unlock spawn rooms if they have been previously selected to allow for reselecting spawn rooms
    if (atkSpawnRoom)
        atkSpawnRoom->setLockedIn(false);
    if (defSpawnRoom)
        defSpawnRoom->setLockedIn(false);

    bool selectingRooms = true;

    int restartTimer = 0;

    while (selectingRooms)
    {
        atkSpawnRoom = nullptr;
        defSpawnRoom = nullptr;

        selectingRooms = false;

        restartTimer += 1;
        if (restartTimer > 5 || selectedRooms.size() <= 0)
        {
            restart = true;
            break;
        }

        int timeout = 0;

        defSpawnRoom = selectedRooms[rand() % selectedRooms.size()];
        // select a new defender spawn if it is too far away from either objective, or the chosen defender spawn is already an objective room
        while (!CheckSpawnConditions(true))
        {
            defSpawnRoom = selectedRooms[rand() % selectedRooms.size()];

            // only try an arbitrary number of rooms, if none fit the criteria then start the entire spawn selection process again
            timeout += 1;
            if (timeout >= 19)
            {
                selectingRooms = true;
                break;
            }
        }

        timeout = 0;

        if (!selectingRooms)
        {
            atkSpawnRoom = selectedRooms[rand() % selectedRooms.size()];
            // select a new attacker spawn if it is too close to either objective or the defender spawn, or the chosen attacker spawn is already an objective room
            while (!CheckSpawnConditions(false))
            {
                atkSpawnRoom = selectedRooms[rand() % selectedRooms.size()];

                // only try an arbitrary number of rooms, if none fit the criteria then start the entire spawn selection process again
                timeout += 1;
                if (timeout >= 19)
                {
                    selectingRooms = true;
                    break;
                }
            }
        }
    }

    if (!restart)
    {
        defSpawnRoom->setLockedIn(true);
        atkSpawnRoom->setLockedIn(true);
    }
       
    UpdateSelectedRooms();

    return !restart;
}

void RoomManager::ConnectRooms()
{
    // get vectors from each spawn room to each other important room
    connectionBoundingRectangles.clear();

    std::vector<sf::Vector2f> defSpawnVectors, atkSpawnVectors;

    // add the important rooms to a separate vector
    finalRooms.clear();
    for (auto room : rooms)
    {
        if (room->isLockedIn())
        {
            finalRooms.push_back(room);
        }
    }

    // calculate the vectors from each spawn room to each objective room
    defSpawnVectors = CalculateVectors(defSpawnRoom);
    atkSpawnVectors = CalculateVectors(atkSpawnRoom);

    // calculate overall mid point of the map
    sf::Vector2f mapMidPoint;
    for (int i = 0; i < finalRooms.size(); i++)
    {
        mapMidPoint += finalRooms[i]->getShape().getPosition();
    }
    mapMidPoint.x /= finalRooms.size();
    mapMidPoint.y /= finalRooms.size();

    // calculate potential connection points (P and Q points) between each spawn room to each objective room
    std::vector<sf::Vector2f> defConnectPoints;
    std::vector<sf::Vector2f> atkConnectPoints;

    for (int i = 0; i < objectiveRooms.size(); i++)
    {
        sf::Vector2f defP, defQ, atkP, atkQ;
        // calculate P and Q points for vector between def spawn and objective rooms
        defP = defSpawnRoom->getShape().getPosition() + sf::Vector2f(defSpawnVectors[i].x, 0.f);
        defQ = defSpawnRoom->getShape().getPosition() + sf::Vector2f(0.f, defSpawnVectors[i].y);

        // calculate P and Q points for vector between atk spawn and objective rooms
        atkP = atkSpawnRoom->getShape().getPosition() + sf::Vector2f(atkSpawnVectors[i].x, 0.f);
        atkQ = atkSpawnRoom->getShape().getPosition() + sf::Vector2f(0.f, atkSpawnVectors[i].y);

        // check which points are farthest from the centre of the map
        // push farthest points onto their respective vector
        if (std::sqrt(std::pow(mapMidPoint.x - defP.x, 2) + std::pow(mapMidPoint.y - defP.y, 2))
         >= std::sqrt(std::pow(mapMidPoint.x - defQ.x, 2) + std::pow(mapMidPoint.y - defQ.y, 2)))
        {
            defConnectPoints.push_back(defP);
        }
        else
        {
            defConnectPoints.push_back(defQ);
        }

        if (std::sqrt(std::pow(mapMidPoint.x - atkP.x, 2) + std::pow(mapMidPoint.y - atkP.y, 2))
         >= std::sqrt(std::pow(mapMidPoint.x - atkQ.x, 2) + std::pow(mapMidPoint.y - atkQ.y, 2)))
        {
            atkConnectPoints.push_back(atkP);
        }
        else
        {
            atkConnectPoints.push_back(atkQ);
        }
    }

    // set up line array to draw connecting lines
    int lineArraySize = 10;
    sf::VertexArray lineArray(sf::LineStrip, lineArraySize);

    lineArray[0] = atkSpawnRoom->getShape().getPosition();//attacker spawn
    lineArray[1] = atkConnectPoints[0];//P1
    lineArray[2] = objectiveRooms[0]->getShape().getPosition();//objective room 1
    lineArray[3] = defConnectPoints[0];//P2
    lineArray[4] = defSpawnRoom->getShape().getPosition();//defender spawn
    lineArray[5] = defConnectPoints[1];//P3
    lineArray[6] = objectiveRooms[1]->getShape().getPosition();//objective room 2
    lineArray[7] = atkConnectPoints[1];//P4
    lineArray[8] = atkSpawnRoom->getShape().getPosition();//attacker spawn
    lineArray[9] = defSpawnRoom->getShape().getPosition();//defender spawn

    // draw bounding rectangles along each connection line
    for (int i = 0; i < lineArraySize - 1; i++)
    {
        sf::RectangleShape connectionBounds;
        sf::Vector2f boundsVector = lineArray[i + 1].position - lineArray[i].position;
        sf::Vector2f boundsCentre = CalculateMidpoint(lineArray[i].position, lineArray[i + 1].position);

        if (boundsVector.y < 0)
        {
            // if the vector is pointing down, invert the vector to be pointing up
            // this is necessary so that the rotation is being applied in the correct direction
            boundsVector = -boundsVector;
        }

        // set the length of the bounding rectangle equal to the distance between the two points
        connectionBounds.setSize(sf::Vector2f(std::sqrt(std::pow(boundsVector.x, 2) + std::pow(boundsVector.y, 2)), 20.f));
        connectionBounds.setOrigin(connectionBounds.getSize() / 2.f);
        connectionBounds.setPosition(boundsCentre);

        // use dot product to get angle between x axis and vector between points and convert to degrees
        float rotation = std::acos(boundsVector.x / std::sqrt(std::pow(boundsVector.x, 2) + std::pow(boundsVector.y, 2)));
        rotation *= 180.f;
        rotation /= 3.14159f;

        connectionBounds.setRotation(rotation);

        connectionBounds.setFillColor(sf::Color(0xff, 0x00, 0x00, 0x77));

        connectionBoundingRectangles.push_back(connectionBounds);
    }

    // lock in all rooms that intersect each connection bounding box
    for (auto room : rooms)
    {
        for (int i = 0; i < connectionBoundingRectangles.size(); i++)
        {
            // getGlobalBounds() returns a non-minimal bounding rectangle meaning that this implementation will encompass more rooms than necessary
            // if the bounding rectangle is rotated (as is the rectangle connecting the defender and attacker spawns), this can be fixed by implementing my 
            // own bounding rectangle implementation however this was caught too close to the deadline to be fixed in time. future work.
            if (room->getShape().getGlobalBounds().intersects(connectionBoundingRectangles[i].getGlobalBounds()))
            {
                room->setSelected(true);
                room->setLockedIn(true);
            }
        }
    }

    UpdateSelectedRooms();

    // delete all rooms that aren't locked in
    finalRooms.clear();
    for (auto room : rooms)
    {
        if (room->isLockedIn())
        {
            finalRooms.push_back(room);
        }
        else
        {
            delete room;
        }
    }
    rooms.clear();
}

void RoomManager::UpdateSelectedRooms()
{
    selectedRooms.clear();

    if (!restart)
    {
        for (auto room : rooms)
        {
            if (room->isSelected())
            {
                // colour the locked in objective rooms as Magenta, the locked in spawn rooms as Blue, and the non-locked selected rooms as cyan
                sf::Color newColor = sf::Color::Cyan;
                if (room->isLockedIn())
                {
                    if (std::find(objectiveRooms.begin(), objectiveRooms.end(), room) != objectiveRooms.end())
                    {
                        newColor = sf::Color::Magenta;
                    }
                    else if (room == atkSpawnRoom || room == defSpawnRoom)
                    {
                        newColor = sf::Color::Blue;
                    }
                }
                room->ChangeColour(newColor);

                selectedRooms.push_back(room);
            }
            else
            {
                room->ChangeColour(sf::Color::Green);
            }
        }
    }
    else
    {
        // if the restart flag is set, clear all room vectors and deselect, and unlock all roomsf
        finalRooms.clear();
        objectiveRooms.clear();
        selectedRooms.clear();

        for (auto room : rooms)
        {
            room->setLockedIn(false);
            room->setSelected(false);
        }
    }
}

bool RoomManager::CheckSpawnConditions(bool defSpawn) 
{
    Room* spawnRoom = defSpawn ? defSpawnRoom : atkSpawnRoom;

    bool conditionsMet = true;

    // if the spawn room is one of the objective rooms, dob't check other conditions and return false
    if (std::find(objectiveRooms.begin(), objectiveRooms.end(), spawnRoom) != objectiveRooms.end())
    {
        conditionsMet = false;
    }

    // if the spawn room is not an objective room, check the distance conditions depending on the type of spawn room
    if (conditionsMet)
    {
        if (defSpawn)
        {
            for (auto objRoom : objectiveRooms)
            {
                // if the defender spawn is too far from any obective room, don't check other objectives and return false
                if (GetDistanceBetweenRooms(spawnRoom, objRoom) > 350)
                {
                    conditionsMet = false;
                    break;
                }
            }
        }
        else
        {

            for (auto objRoom : objectiveRooms)
            {
                // if the attacker spawn is too close to the defender spawn, return false
                if (GetDistanceBetweenRooms(atkSpawnRoom, defSpawnRoom) < 500)
                {
                    conditionsMet = false;
                    break;
                }

                // if the attacker spawn is too close to either objective room, don't check other objectives and return false
                if (GetDistanceBetweenRooms(spawnRoom, objRoom) < 400)
                {
                    conditionsMet = false;
                    break;
                }
            }
        }
    }

    return conditionsMet;
}

// calculate vectors between any room and the objective rooms
std::vector<sf::Vector2f> RoomManager::CalculateVectors(Room* room)
{
    std::vector<sf::Vector2f> vectors;
    sf::Vector2f vect;

    for (auto objRoom : objectiveRooms)
    {
        // calculate vectors from selected room to each objective room
        vect = objRoom->getShape().getPosition() - room->getShape().getPosition();
        vectors.push_back(vect);
    }

    return vectors;
}

sf::Vector2f RoomManager::GetRandomPointInCircle()
{
    float t = 2 * 3.14159 * ((double)rand() / (RAND_MAX));
    float u = ((double)rand() / (RAND_MAX)) + ((double)rand() / (RAND_MAX));
    float r = NULL;

    if (u > 1)
        r = 2 - u;
    else
        r = u;

    sf::Vector2f randPoint(circleRad * r * std::cos(t) + circleCentre.x, circleRad * r * std::sin(t) + circleCentre.y);

    return randPoint;
}

float RoomManager::GetDistanceBetweenRooms(Room* r1, Room* r2)
{
    sf::Vector2f r1mid = r1->getShape().getPosition();
    sf::Vector2f r2mid = r2->getShape().getPosition();

    return std::sqrt(std::pow(r2mid.x - r1mid.x, 2) + std::pow(r2mid.y - r1mid.y, 2));
}

sf::Vector2f RoomManager::CalculateMidpoint(sf::Vector2f a, sf::Vector2f b)
{
    return sf::Vector2f((a.x + b.x)/2, (a.y + b.y)/2);
}

void RoomManager::ClearRooms()
{
    for (auto room : rooms)
    {
        delete room;
    }
    rooms.clear();
}
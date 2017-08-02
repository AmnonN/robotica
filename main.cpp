#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "NodeMap/NodeMap.h"
#include "PathPlanner.h"
#include <stdlib.h>
#include "Gui/MapDrawer.h"
#include "Localization/LocalizationManager.h"
#include "Structs.h"
#include "Robot.h"
#include "MovementManager.h"
#include "Utils/PositionUtils.h"
#include "math.h"

using namespace std;
using namespace HamsterAPI;

void startRobotAction();

int main() {
	try {
		startRobotAction();
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

bool isWaypointReached(const positionState& currLocation,
		const Node& hamsterWaypoint) {
	double distanceFromWaypoint = sqrt(
			pow(currLocation.pos.x - hamsterWaypoint.getX(), 2)
					+ pow(currLocation.pos.y - hamsterWaypoint.getY(), 2));

	return distanceFromWaypoint <= DISTANCE_FROM_WAYPOINT_TOLERANCE;
}

void startRobotAction()
{
 cv::namedWindow("Room-Map");
 HamsterAPI::Hamster hamster(1);
 sleep(3);

 NodeMap roomRealMap;
 NodeMap roomBlownMap;
 OccupancyGrid roomRealMapFromMemory = hamster.getSLAMMap();
 MapDrawer mapDrawer(roomRealMapFromMemory.getWidth(), roomRealMapFromMemory.getHeight());

 // Init NodeMaps
 mapDrawer.DrawMap(&roomRealMapFromMemory, 0);

 cv::Mat* drawedMap = mapDrawer.getMap();
 AngleUtils::rotateMapOnOrigin(drawedMap, MAP_ROTATION);

 // AmnonN fix the weird matrix inside
 roomRealMap.loadMap(drawedMap);
 roomBlownMap.loadBlowMap(drawedMap);

 // Init the robot start and goal positions
 Node *startPos = roomBlownMap.getNodeAtIndex(ROBOT_START_X, ROBOT_START_Y);
 Node *goalPos = roomBlownMap.getNodeAtIndex(GOAL_X, GOAL_Y);

 PathPlanner *pathPlanner;
 pathPlanner->findShortestPath(&roomBlownMap,startPos,goalPos);
 std::list<Node* > waypoints = pathPlanner->markWaypoints(startPos, goalPos);

// int waypointCouinter = 0;
// for (std::list<Node*>::reverse_iterator iter = waypoints.rbegin(); iter != waypoints.rend(); ++iter)
//	{
//	 Node* currWaypoint = *iter;
//	 cout << "waypoint #" << ++waypointCouinter << ": (" << currWaypoint->getX() << ", " << currWaypoint->getY() << ")" << endl;
//	}

 mapDrawer.DrawMap(&roomRealMapFromMemory, MAP_ROTATION);
 mapDrawer.DrawNodeMap(&roomBlownMap);
 mapDrawer.DrawPath(goalPos);
 mapDrawer.SaveCurrentMap();

 Pose robotStartPose = hamster.getPose();
 struct position startPosition = {.x =
		 ROBOT_START_X + robotStartPose.getX(), .y = ROBOT_START_Y -  robotStartPose.getX()};\
 struct positionState startPositionState = {.pos = startPosition, .yaw = robotStartPose.getHeading()};

 LocalizationManager* localizationManager = new LocalizationManager(&roomRealMapFromMemory, &hamster, NULL, &roomBlownMap);

 double mapResolution = roomRealMapFromMemory.getResolution();
 int inflationFac = ROBOT_SIZE / 2 / (roomRealMapFromMemory.getResolution() * 100);

 Robot robot(&hamster,localizationManager, inflationFac, mapResolution);
 localizationManager->robot = &robot;

MovementManager movementManager(&hamster, &robot, &mapDrawer, localizationManager);

// TODO : Check this later
float deltaX = robot.GetDeltaX();
float deltaY = robot.GetDeltaY();
float deltaYaw = robot.GetDeltaYaw();

if(hamster.isConnected()) {
 for (std::list<Node*>::reverse_iterator iter = waypoints.rbegin(); iter != waypoints.rend(); ++iter)
	{
	 	//Node* temp = new Node ((*iter)->getY(), (*iter)->getX());
		Node* currWaypoint = *iter;


		Node hamsterWaypoint = ConvertToHamsterLocation(currWaypoint);
		positionState currLocation = robot.GetCurrHamsterLocation();

		if (isWaypointReached(currLocation, hamsterWaypoint))
		{
			cout << endl << "Reached waypoint (" << hamsterWaypoint.getX() << ", " << hamsterWaypoint.getY() << ")" << endl << endl;
		}
		else
		{
			LidarScan lidar = hamster.getLidarScan();
			localizationManager->Update(deltaX, deltaY, deltaYaw, &lidar, &roomBlownMap);
			movementManager.NavigateToWaypoint(&hamsterWaypoint, lidar);
		}

		deltaX = robot.GetDeltaX();
		deltaY = robot.GetDeltaY();
		deltaYaw = robot.GetDeltaYaw();
	}

 	 cout << "The Robot reached the waypoint: (" << GOAL_X << ", " << GOAL_Y << ")" << endl;
}

}




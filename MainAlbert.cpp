/*
#include "Utils/PngUtils.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include "Configuration/ParameterSerialization.h"
#include "PathPlanner.h"
#include "NodeMap/MapConverter.h"
#include "Robot.h"
#include "Constants.h"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned imageWidth;
	unsigned imageHeight;
	vector<unsigned char> originalImage;


	// Check the number of parameters
	if (argc < 2)
	{
		// Tell the user how to run the program
		cerr << "Usage: " << argv[0] << " <PARAMETER_FILE_PATH>" << endl;

		return 1;
	}

	char* parameterFilePath = argv[1];

	struct parameters settings = deserializeParametersFile(parameterFilePath);

	cout << "Deserialized the configuration file" << endl;

	// Pixels per cell = Centimeters per cell / Centimeters per pixel
	int pixelsPerCell = settings.gridResolutionCM / settings.mapResolutionCM;

	struct position resizedStart;

	resizedStart.x = (int)(settings.startLocation.pos.x / pixelsPerCell);
	resizedStart.y = (int)(settings.startLocation.pos.y / pixelsPerCell);

	struct position resizedGoal;

	resizedGoal.x = (int)(settings.goal.x / pixelsPerCell);
	resizedGoal.y = (int)(settings.goal.y / pixelsPerCell);

	cout << "Resized the settings" << endl;

	decodeOneStep(originalImage, imageWidth, imageHeight, settings.mapFilePath);

	cout << "Read the original image" << endl;

	vector<unsigned char> blownImage = blowImage(originalImage, settings.robotSize.width,
			settings.mapResolutionCM, imageWidth, imageHeight);

	cout << "Blown the image" << endl;

	if (DEBUG_BLOWN_MAP)
	{
		// Saves the blown image to the disk
		encodeOneStep(BLOWN_IMAGE_DEBUG_MAP, blownImage, imageWidth, imageHeight);

		cout << "Successfully wrote the blown picture to the disk" << endl;
	}

	NodeMap originalMap;

	originalMap.loadMap(originalImage, imageWidth, imageHeight);

	NodeMap blownMap;

	blownMap.loadMap(blownImage, imageWidth, imageHeight);

	cout << "Loaded the original map Width " << blownMap.getWidth() <<
		" Height " << blownMap.getHeight() << endl;

	NodeMap resizedMap;

	blownMap.resizeMap(pixelsPerCell, &resizedMap);

	cout << "Loaded the resized map Width " << resizedMap.getWidth() << " Height " << resizedMap.getHeight() << endl;

	Node* startNode = resizedMap.getNodeAtIndex(resizedStart.x, resizedStart.y);

	Node* goalNode = resizedMap.getNodeAtIndex(resizedGoal.x, resizedGoal.y);

	findShortestPath(&resizedMap, startNode, goalNode);

	cout << "Found the shortest path" << endl;

	set<Node*> waypoints;

	markWaypoints(&resizedMap, &originalMap, resizedStart, resizedGoal, pixelsPerCell, &waypoints);

	cout << "Marked the waypoints on the original map" << endl;

	vector<unsigned char> outputImage = convertMapToImage(&originalMap, settings.startLocation.pos,
			settings.goal, imageWidth, imageHeight);

	cout << "Converted the final map to an image" << endl;

	// Debugging

	if (DEBUG_WAYPOINTS_MAP)
	{
		encodeOneStep(WAYPOINTS_DEBUG_IMAGE_NAME, outputImage, imageWidth, imageHeight);

		cout << "Wrote the original image to the disk" << endl;
	}

	if (DEBUG_RESIZED_PATH_MAP)
	{
		vector<unsigned char> resizedImage = convertMapToImage(&resizedMap, resizedStart,
						resizedGoal, resizedMap.getWidth(), resizedMap.getHeight());

		encodeOneStep(RESIZED_PATH_DEBUG_IMAGE_NAME, resizedImage,
				imageWidth / pixelsPerCell, imageHeight / pixelsPerCell);

		cout << "Wrote the resized image to the disk" << endl;
	}

	cout << "Starting to move towards the goal" << endl;

	Robot robot(SERVER_HOST, SERVER_PORT, settings, &waypoints, &originalMap);

	robot.run();
}
*/

/*
#include "MapConverter.h"
#include "../Utils/PngUtils.h"

vector<unsigned char> convertMapToImage(NodeMap* map, struct position start, struct position goal,
		unsigned width, unsigned height)
{
	unsigned x;
	unsigned y;

	vector<unsigned char> outputImage;
	outputImage.resize(width * height * 4);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			outputImage[y * width * 4 + x * 4 + 3] = 255; // a

			Node* currNode = map->getNodeAtIndex(x, y);

			if (currNode->getIsObstacle())
			{
				// Color black
				outputImage[y * width * 4 + x * 4 + 0] = 0; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
			else if (!currNode->getIsWaypoint())
			{
				// Color white
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 255; // g
				outputImage[y * width * 4 + x * 4 + 2] = 255; // b
			}
			else
			{
				// Color red
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
		}
	}

	// Color green
	colorArea(&outputImage, width, height, start, 0, 255, 0);

	// Color blue
	colorArea(&outputImage, width, height, goal, 0, 0, 255);

	// Mark the route to the goal
	Node* currNode = map->getNodeAtIndex(goal.x, goal.y);

	while (currNode->getParent() != NULL)
	{
		// Color red
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 0] = 255; // r
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 1] = 0; // g
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 2] = 0; // b

		currNode = currNode->getParent();
	}

	return outputImage;
}

// For debug use
vector<unsigned char> convertScannedMapToImage(NodeMap* map, struct position start, struct position goal,
		unsigned width, unsigned height, set<Node*>* openList, set<Node*>* closedList, Node* curr)
{
	unsigned x;
	unsigned y;

	vector<unsigned char> outputImage;
	outputImage.resize(width * height * 4);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			outputImage[y * width * 4 + x * 4 + 3] = 255; // a

			if (map->getNodeAtIndex(x, y)->getIsObstacle())
			{
				// Color black
				outputImage[y * width * 4 + x * 4 + 0] = 0; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
			else
			{
				// Color white
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 255; // g
				outputImage[y * width * 4 + x * 4 + 2] = 255; // b
			}
		}
	}

	// Color green
	colorArea(&outputImage, width, height, start, 0, 255, 0);

	// Color blue
	colorArea(&outputImage, width, height, goal, 0, 0, 255);

	// Mark the route to the goal
	Node* currNode = map->getNodeAtIndex(goal.x, goal.y);

	while (currNode->getParent() != NULL)
	{
		// Color red
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 0] = 255; // r
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 1] = 0; // g
		outputImage[currNode->getY() * width * 4 + currNode->getX() * 4 + 2] = 0; // b

		currNode = currNode->getParent();
	}

	// Color the open list
	for (set<Node*>::iterator iter = openList->begin(); iter != openList->end(); iter++)
	{
		Node* node = *iter;

		struct position p;
		p.x = node->getX();
		p.y = node->getY();

		// Purple
		colorPixel(&outputImage, width, height, p, 128, 0, 128);
	}

	// Color the closed list
	for (set<Node*>::iterator iter = closedList->begin(); iter != closedList->end(); iter++)
	{
		Node* node = *iter;

		struct position p;
		p.x = node->getX();
		p.y = node->getY();

		// Yellow
		colorPixel(&outputImage, width, height, p, 255, 255, 0);
	}

	// Color the current node
	struct position p;
	p.x = curr->getX();
	p.y = curr->getY();

	colorSmallArea(&outputImage, width, height, p, 255, 192, 203);


	return outputImage;
}

// For debug use
vector<unsigned char> convertObstacleMapToImage(NodeMap* map, struct position robotLocation,
		vector<struct positionState>* obstacles)
{
	unsigned x;
	unsigned y;

	unsigned width = map->getWidth();
	unsigned height = map->getHeight();

	vector<unsigned char> outputImage;
	outputImage.resize(map->getWidth() * map->getHeight() * 4);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			outputImage[y * width * 4 + x * 4 + 3] = 255; // a

			if (map->getNodeAtIndex(x, y)->getIsObstacle())
			{
				// Color black
				outputImage[y * width * 4 + x * 4 + 0] = 0; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
			else
			{
				// Color white
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 255; // g
				outputImage[y * width * 4 + x * 4 + 2] = 255; // b
			}
		}
	}

	for (unsigned index = 0; index < obstacles->size(); index++)
	{
		// Color green
		colorPixel(&outputImage, width, height, (*obstacles)[index].pos, 0, 255, 0);
	}

	// Color red
	colorSmallArea(&outputImage, map->getWidth(), map->getHeight(), robotLocation, 255, 0, 0);


	return outputImage;
}

vector<unsigned char> convertParticleMapToImage(NodeMap* map, vector<struct position>* particles)
{
	unsigned x;
	unsigned y;

	unsigned width = map->getWidth();
	unsigned height = map->getHeight();

	vector<unsigned char> outputImage;
	outputImage.resize(map->getWidth() * map->getHeight() * 4);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			outputImage[y * width * 4 + x * 4 + 3] = 255; // a

			if (map->getNodeAtIndex(x, y)->getIsObstacle())
			{
				// Color black
				outputImage[y * width * 4 + x * 4 + 0] = 0; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
			else
			{
				// Color white
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 255; // g
				outputImage[y * width * 4 + x * 4 + 2] = 255; // b
			}
		}
	}

	for (unsigned int particleIndex = 0; particleIndex < particles->size(); particleIndex++)
	{
		struct position currParticle = (*particles)[particleIndex];

		// Color green
		colorPixel(&outputImage, width, height, currParticle, 0, 255, 0);
	}

	return outputImage;
}

vector<unsigned char> convertRunMapToImage(NodeMap* map, struct position start, struct position goal,
		struct position robotLocation, list<struct position>* particles, set<Node*>* waypoints, struct position currWaypoint)
{
	unsigned x;
	unsigned y;

	unsigned width = map->getWidth();
	unsigned height = map->getHeight();

	vector<unsigned char> outputImage;
	outputImage.resize(map->getWidth() * map->getHeight() * 4);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			outputImage[y * width * 4 + x * 4 + 3] = 255; // a

			if (map->getNodeAtIndex(x, y)->getIsObstacle())
			{
				// Color black
				outputImage[y * width * 4 + x * 4 + 0] = 0; // r
				outputImage[y * width * 4 + x * 4 + 1] = 0; // g
				outputImage[y * width * 4 + x * 4 + 2] = 0; // b
			}
			else
			{
				// Color white
				outputImage[y * width * 4 + x * 4 + 0] = 255; // r
				outputImage[y * width * 4 + x * 4 + 1] = 255; // g
				outputImage[y * width * 4 + x * 4 + 2] = 255; // b
			}
		}
	}

	// Color the start location purple
	colorSmallArea(&outputImage, map->getWidth(), map->getHeight(), start, 255, 0, 255);

	// Color the goal location blue
	//colorSmallArea(&outputImage, map->getWidth(), map->getHeight(), goal, 0, 0, 255);

	// Color the current robot location red
	colorSmallArea(&outputImage, map->getWidth(), map->getHeight(), robotLocation, 255, 0, 0);

	// Color the waypoints navy
	for (set<Node*>::iterator it = waypoints->begin(); it != waypoints->end(); it++)
	{
	    Node* currWaypoint = *it;

	    struct position currPos;
	    currPos.x = currWaypoint->getX();
	    currPos.y = currWaypoint->getY();

		// Color navy
		colorPixel(&outputImage, width, height, currPos, 0, 0, 128);
	}

	// Color the next waypoint dark green
	colorPixel(&outputImage, width, height, currWaypoint, 0, 128, 0);

	// Color the particles green
	for(list<struct position>::iterator it = particles->begin(); it != particles->end(); it++)
	{
		struct position currParticle = *it;

		// Color green
		colorPixel(&outputImage, width, height, currParticle, 0, 255, 0);
	}

	return outputImage;
}
*/

/*

 * MapConverter.h
 *
 *  Created on: Jul 8, 2016
 *      Author: user


#ifndef MAPCONVERTER_H_
#define MAPCONVERTER_H_

#include "NodeMap.h"
#include <set>
#include "../Structs.h"
#include <list>

using namespace std;

vector<unsigned char> convertMapToImage(NodeMap* map, struct position start, struct position goal,
		unsigned width, unsigned height);

// For debug use
vector<unsigned char> convertScannedMapToImage(NodeMap* map, struct position start, struct position goal,
		unsigned width, unsigned height, set<Node*>* openList, set<Node*>* closedList, Node* curr);

// For debug use
vector<unsigned char> convertObstacleMapToImage(NodeMap* map, struct position robotLocation,
		vector<struct positionState>* obstacles);

// For debug use
vector<unsigned char> convertParticleMapToImage(NodeMap* map, vector<struct position>* particles);

// For debug use
vector<unsigned char> convertRunMapToImage(NodeMap* map, struct position start, struct position goal,
		struct position robotLocation, list<struct position>* particles, set<Node*>* waypoints, struct position currWaypoint);

#endif  MAPCONVERTER_H_
*/

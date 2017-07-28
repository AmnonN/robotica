#ifndef GUI_MAPDRAWER_H_
#define GUI_MAPDRAWER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "MapPointType.h"
#include "../NodeMap/Node.h"
#include "../NodeMap/NodeMap.h"
#include "../Localization/LocalizationParticle.h"
#include "../Utils/PositionUtils.h"
#include "../Utils/AngleUtils.h"
#include "opencv2/imgproc.hpp"
using namespace HamsterAPI;
using namespace std;

class MapDrawer
{
public:
	MapDrawer(int width, int height);

	void SetPointType(size_t x, size_t y, MapPointType mapPointType);
	void DrawMap(const OccupancyGrid* occupancyGridMap, double rotationAngle);
	void DrawNodeMap(NodeMap* nodeMap);
	void DrawPath(Node* goal);
	void Show();
	void DrawPatricles(std::vector<LocalizationParticle *>* particles);
	cv::Mat* getMap();

private:
    const string WINDOW_TITLE;
    cv::Mat* _map;
    void SetPointColor(size_t x, size_t y, int red, int green, int blue);
};

#endif /* GUI_MAPDRAWER_H_ */

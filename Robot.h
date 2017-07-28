#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <set>
#include <math.h>
#include "NodeMap/Node.h"
#include "NodeMap/NodeMap.h"
#include "NodeMap/MapConverter.h"
#include "Structs.h"
#include "Gui/MapDrawer.h"
#include "Constants.h"
#include "Utils/AngleUtils.h"
#include "Localization/LocalizationManager.h"

using namespace HamsterAPI;
using namespace std;

class Robot {
private:
	Hamster * hamster;
	double hamsterStartX, hamsterStartY;
	LocalizationManager * localizationManager;
	double prevX, prevY, prevYaw, currX, currY, currYaw, resolution;
	int inflationRadius;
	double mapHeight, mapWidth;

public:
	Robot(Hamster * hamster, LocalizationManager * localizationManager, int inflationRadius , double resolution);
	void Initialize(positionState startLocation);
	double GetDeltaX() const;
	double GetDeltaY() const;
	double GetDeltaYaw() const;
	positionState GetCurrHamsterLocation();
	void UpdateLocation();
	virtual ~Robot();
};

#endif /* ROBOT_H_ */

#ifndef LASER_H_
#define LASER_H_

#include <vector>
#include <math.h>
#include "../Utils/AngleUtils.h"
#include "../Structs.h"
#include "../Constants.h"
#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

class LidarHandler
{
	private:
		double _pixelResolution;
		HamsterAPI::Hamster* _hamster;
		double _minAngle;
		double _maxAngle;
		double convertSampleIndexToDegree(int sampleIndex) const;
		int convertDegreeToSampleIndex(double degree) const;

	public:
		LidarHandler(HamsterAPI::Hamster* hamster, double mapResolution, double minScanAngle, double maxScanAngle);
		void getObstacles(double robotX, double robotY, double robotYawDegrees,
			vector<struct positionState>* obstacles) const;
};

#endif // LASER_H_

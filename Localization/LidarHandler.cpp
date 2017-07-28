#include "LidarHandler.h"

LidarHandler::LidarHandler(HamsterAPI::Hamster* hamster, double pixelResolution, double minScanAngle, double maxScanAngle)
{
	_pixelResolution = pixelResolution;
	_hamster = hamster;

	_minAngle = minScanAngle;
	_maxAngle = maxScanAngle;
}

void LidarHandler::getObstacles(double robotX, double robotY, double robotYawDegrees,
	vector<struct positionState>* obstacles) const
{
	HamsterAPI::LidarScan scan = _hamster->getLidarScan();
	for (size_t i = 0; i < scan.getScanSize(); i++)
	{
		double degree = scan.getScanAngleIncrement() * i;

		if(degree >= _minAngle && degree <= _maxAngle) {
			double sampleDistanceMeters = scan.getDistance(i);

			// Obstacle found
			if (sampleDistanceMeters < scan.getMaxRange() - 2)
			{
				double sampleDistancePixels = sampleDistanceMeters * 100 / _pixelResolution;

				while (degree >= 360)
				{
					degree -= 360;
				}

				double degreeInRadians = AngleUtils::convertDegreesToRadians(degree);

				double obstacleX = robotX - sampleDistancePixels * sin(degreeInRadians);
				double obstacleY = robotY + sampleDistancePixels * cos(degreeInRadians);

				struct positionState obstaclePosition;
				obstaclePosition.pos.x = obstacleX;
				obstaclePosition.pos.y = obstacleY;
				obstaclePosition.yaw = degree;

				obstacles->push_back(obstaclePosition);
			}
		}
	}
}


#include "LocalizationManager.h"
#include <iostream>
#include <algorithm>

using namespace std;



LocalizationManager::LocalizationManager( OccupancyGrid *ogrid, Hamster *hamster)
{
	this->hamster = hamster;
	this->ogrid = ogrid;
}

void LocalizationManager::createRandomParticle(LocalizationParticle *par)
{
	//Randomizing an angle
	par->yaw = rand() % 360;

	//set random column and row while the random cell chosen isn't free
	do {
		par->col = rand() % ogrid->getWidth();
		par->row = rand() % ogrid->getHeight();

	} while (ogrid->getCell(par->row, par->col) != CELL_FREE);

	//Conversion
	par->x = (par->col - (double) ROBOT_START_X)* ogrid->getResolution();
	par->y = ((double) ROBOT_START_Y - par->row)* ogrid->getResolution();

}
double LocalizationManager:: randNumberFactor(int level)
{
	if(level == 3)
		return 0.4 -0.8*(double)rand()/(double)RAND_MAX;
	else if(level == 2)
		return 0.2-0.4*(double)rand()/(double)RAND_MAX;
	else
		return 0.1-0.2*(double)rand()/(double)RAND_MAX;
}

double LocalizationManager:: randNumberFactorYaw(int level)
{
	if(level == 5)
		return 180 - rand() % 360;
	else if(level == 4)
		return 90 - rand() % 180;
	else if(level == 3)
		return 30 - rand() % 60;
	else if(level == 2)
		return 10 - rand() % 20;
	else
		return 5 - rand() % 10;

}

void LocalizationManager::createNeighborParticales(LocalizationParticle *badParticale,  LocalizationParticle *goodParticale)//previous particle , new particle
{
	do {
		if (goodParticale->belief < 0.3)
		{
			badParticale->x = goodParticale->x+ randNumberFactor(3);
			badParticale->y = goodParticale->y + randNumberFactor(3);
		}
		else if (goodParticale->belief < 0.6)
		{
			badParticale->x = goodParticale->x+ randNumberFactor(2);
			badParticale->y = goodParticale->y+ randNumberFactor(2);
		}
		else
		{
			badParticale->x = goodParticale->x+ randNumberFactor(1);
			badParticale->y = goodParticale->y+ randNumberFactor(1);
		}

		badParticale->row = (double) ROBOT_START_Y - badParticale->y / ogrid->getResolution();
		badParticale->col = badParticale->x / ogrid->getResolution()+ ROBOT_START_X;

	} while (ogrid->getCell(badParticale->row, badParticale->col) != CELL_FREE);

	if (goodParticale->belief < 0.2)
		badParticale->yaw = (goodParticale->yaw + (randNumberFactorYaw(5)));
	else if (goodParticale->belief < 0.4)
		badParticale->yaw = (goodParticale->yaw + (randNumberFactorYaw(4)));
	else if (goodParticale->belief < 0.6)
		badParticale->yaw = (goodParticale->yaw + (randNumberFactorYaw(3)));
	else if (goodParticale->belief < 0.8)
		badParticale->yaw = (goodParticale->yaw + (randNumberFactorYaw(2)));
	else
		badParticale->yaw = (goodParticale->yaw + (randNumberFactorYaw(1)));

	if(badParticale->yaw >= 360)
		badParticale->yaw -= 360;
	if(badParticale->yaw < 0)
		badParticale->yaw += 360;


}




void LocalizationManager::InitParticalesOnMap(positionState * ps)
{
	particles.resize(NUM_OF_PARTICALES);

	initSourceParticle(ps);

	for (size_t i = 0; i < particles.size() - 1 ; i++)
	{
		particles[i] = new LocalizationParticle();

		//Randomizing an angle
		double degYaw = ps->yaw*180/M_PI + 180;
		if(degYaw >= 360)
			degYaw -= 360;
		if(degYaw < 0)
			degYaw += 360;

		particles[i]->yaw = rand() % 360;

		//set random column and row while the random cell chosen isn't free
		do {
			particles[i]->col = ps->pos.x + rand() % 5 ;
			particles[i]->row = ps->pos.y + rand() % 5;

		} while (ogrid->getCell(particles[i]->row, particles[i]->col) != CELL_FREE);

		//Conversion
		particles[i]->x = (particles[i]->col - (double) ROBOT_START_X)*  ogrid->getResolution();
		particles[i]->y = ((double) ROBOT_START_Y - particles[i]->row)* ogrid->getResolution();

	}


}

void LocalizationManager::initSourceParticle(positionState * ps) {
	particles[particles.size() - 1] = new LocalizationParticle();
	particles[particles.size() - 1]->col = ps->pos.x;
	particles[particles.size() - 1]->row = ps->pos.y;
	particles[particles.size() - 1]->yaw = ps->yaw;
	particles[particles.size() - 1]->belief = 1;
}

double LocalizationManager::updateBelief(LocalizationParticle *p)
{
	LidarScan scan = hamster->getLidarScan();

	int hits = 0;
	int misses = 0;

	for (unsigned int i = 0; i < scan.getScanSize(); i++)
	{
		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

		if (scan.getDistance(i) < scan.getMaxRange() - 0.001)
		{

			double wallX = p->x + scan.getDistance(i)* cos(angle + p->yaw * DEG2RAD- 180 * DEG2RAD);


			double wallY = p->y+ scan.getDistance(i)* sin(angle + p->yaw * DEG2RAD- 180 * DEG2RAD);


			int i = (double) ROBOT_START_Y - wallY / ogrid->getResolution();


			int j = wallX / ogrid->getResolution() + ROBOT_START_X;



			if (ogrid->getCell(i, j) == CELL_OCCUPIED)
			{
				hits++;
			}
			else
			{
				misses++;
			}
		}
	}




	return (double) hits / (hits + misses);
}

bool compareParticals(LocalizationParticle* x, LocalizationParticle* y)
{
	if(x->belief < y->belief)
		return true;
	return false;
}

bool LocalizationManager::tryReturnBackOutOfRangeParticle(LocalizationParticle *p)
{
	LocalizationParticle * copyPar = new LocalizationParticle(*p);
	int distant;
	int count = 0;
	do {
		//+-7 for distant
		distant = 14 - rand() % 28;
		p->col = copyPar->col + distant;
		distant = 14 - rand() % 28;
		p->row = copyPar->row + distant;
		count++;

	} while (ogrid->getCell(p->row, p->col) != CELL_FREE && count < TRY_TO_BACK);

	p->x = (p->col - (double) ROBOT_START_X) * ogrid->getResolution();
	p->y = ((double) ROBOT_START_Y - p->row) * ogrid->getResolution();
	delete copyPar;

	return count < TRY_TO_BACK;
}

void LocalizationManager::calculateYaw(LocalizationParticle* p, double deltaYaw) {
	p->yaw += deltaYaw;
	if (p->yaw >= 360) {
		p->yaw -= 360;
	}
	if (p->yaw < 0) {
		p->yaw += 360;
	}
}

void LocalizationManager::calculateRealPos(LocalizationParticle* p,
										   double deltaX,
										   double deltaY,
										   double deltaYaw) {
	double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	p->x += distance * cos(p->yaw * DEG2RAD);
	p->y += distance * sin(p->yaw * DEG2RAD);

	calculateYaw(p, deltaYaw);
}

void LocalizationManager::calculatePositionOnMap(LocalizationParticle* p) {
	p->row = (double) ROBOT_START_Y - p->y / ogrid->getResolution();
	p->col = p->x / ogrid->getResolution() + ROBOT_START_X;
}

void LocalizationManager::replaceBadOutOfRangeParticle(LocalizationParticle* p, int size) {
	int indexFromTop = size - rand() % TOP_PARTICALES - 1;

	if (particles[indexFromTop]->belief > 0.4)
	{
		createNeighborParticales(p, particles[indexFromTop]);
	}
	else
	{
		createRandomParticle(p);
	}
}

void LocalizationManager:: moveParticales(double deltaX, double deltaY, double deltaYaw)
{
	int size = particles.size();
	for (size_t i = 0; i < particles.size(); i++)
	{
		LocalizationParticle *p = particles[i];

		calculateRealPos(p, deltaX, deltaY, deltaYaw);
		calculatePositionOnMap(p);

		if (ogrid->getCell(p->row, p->col) != CELL_FREE &&
			(p->belief <= MIN_BELIEF || tryReturnBackOutOfRangeParticle(p)))
		{
			replaceBadOutOfRangeParticle(p, size);
		}

		p->belief = updateBelief(p);
	}

	std::sort(particles.begin(), particles.end(), compareParticals);

	for (int i = 1; i <= BAD_PARTICALES; i++)
	{
		if (particles[size - i]->belief > MIN_BELIEF)
		{
			createNeighborParticales(particles[i - 1], particles[size - i]);
			updateBelief(particles[i - 1]);
		}
		else
		{
			createRandomParticle(particles[i - 1]);
			updateBelief(particles[i - 1]);
		}
	}

}

void LocalizationManager::printParticles()
{
	for (unsigned int i = 0; i < particles.size(); i++)
	{

		cout << "Particle's Number " << i <<": " << endl;
		cout<< "x : "<<particles[i]->x <<endl;
		cout<< "y : "<< particles[i]->y<<endl;
		cout<< "heading angle : " << particles[i]->yaw <<endl;
		cout<<"belief : "<< particles[i]->belief << endl<<endl<<endl;
	}
}

vector<LocalizationParticle *>* LocalizationManager::getParticles()
{
	return &particles;
}

positionState LocalizationManager::getPosition() {
	LocalizationParticle* localizationParticle = particles[particles.size() -1];

	positionState positionState;
	positionState.pos.x = localizationParticle->col;
	positionState.pos.y = localizationParticle->row;
	positionState.yaw = localizationParticle->yaw;

	return positionState;
}

double LocalizationManager::getBestBelief() {
	return particles[particles.size() -1]->belief;
}

LocalizationManager::~LocalizationManager()
{
}


#include <omnetpp.h>

#include "WindManager.h"
#include "BaseModule.h"

Define_Module(WindManager);

WindManager::WindManager()
{
	// TODO Auto-generated constructor stub

}

WindManager::~WindManager()
{
	// TODO Auto-generated destructor stub
}

void WindManager::initialize(int stage)
{
	BaseModule::initialize(stage);

	EV << "Initializing WindManager stage " << stage << endl;

	if (stage == 0)
	{
	        windDirection = par("windDirection");
	        windSpeed = par("windSpeed");

	        windDirChange = par("windDirChange");
	        windSpeedChange = par("windSpeedChange");
	}
	else if (stage == 1)
	{

		for(uint i=0; i<2000; i+=100)
		{
			EV << i << " " << getWind(Coord(0,0,i)).info() << endl;
		}

	}
}

Coord WindManager::getWind(Coord pos)
{
	Coord wind;
	double windd = pos.z/1000.0 * windDirChange + windDirection;
	double windv = pos.z/1000.0 * windSpeedChange;

	if(windv < 0)
		windv = 0;
	windv += windSpeed;

	double angle = windd - 90;

	wind.x = -cos(PI * angle / 180) * windv;
	wind.y = -sin(PI * angle / 180) * windv;
	wind.z = 0;

	return wind;
}

//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <fstream>

#include <omnetpp.h>

#include "BaseModule.h"
#include "FindModule.h"

#include "ThermalManager.h"
#include "ThermalChildress.h"
#include "WindManager.h"


Define_Module(ThermalManager);

ThermalManager::ThermalManager()
{
	// TODO Auto-generated constructor stub

}

ThermalManager::~ThermalManager()
{
	// TODO Auto-generated destructor stub
}

void ThermalManager::initialize(int stage)
{
	BaseModule::initialize(stage);

	EV << "Initializing ThermalManager stage " << stage << endl;

	if (stage == 0)
	{
		numThermals = par("numThermals");
	}
	else if (stage == 1)
	{
		thermals.push_back(ThermalChildress(Coord(2500, 2500, 0), 2000, NO_DOWNDRAFT));

		ThermalChildress thermal = thermals.front();

		std::ofstream results;
 		results.open("results/thermal.csv");
		results << "x,y,z,updraft" << endl;

		//int x = 2500;
		for(int x=2000; x<3000; x+=10)
		{
			for(int z=0; z<1600; z+=10)
			{
				double updraft = thermal.upDraft(Coord(x,2500,z));
				results << x << "," << "2500" << "," << z << "," << updraft << endl;
				//EV<<  "z: " << z << " updraft: " << updraft << endl;
			}
		}
		results.close();

 		results.open("results/thermalpos.csv");
		results << "x,y,z" << endl;

		for(int z=0; z<3000; z+=1)
		{
			Coord pos = thermal.positionAtAltitude(z);
			results << pos.x << "," << pos.y << "," << pos.z << endl;
			//EV<<  "z: " << z << " updraft: " << updraft << endl;
		}
		results.close();
	}
}

Coord ThermalManager::getAirFlow(Coord gliderPos)
{
	double z = 0;

	for (std::list<ThermalChildress>::iterator list_iter = thermals.begin(); list_iter != thermals.end(); list_iter++)
	{
		z += list_iter->upDraft(gliderPos);
	}

	return Coord(0,0,z);
}


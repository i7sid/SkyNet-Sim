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

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include <fstream>
#include <iostream>
#include <string>
#include <FWMath.h>


#include "LinearMobility.h"
//#include "BaseMobility.h"
#include "GliderMobilityA.h"
#include "ThermalManager.h"
#include "FindModule.h"

Define_Module(GliderMobilityA);

GliderMobilityA::GliderMobilityA()
{

}

GliderMobilityA::~GliderMobilityA()
{
	EV << "node[" << getParentModule()->getIndex() << "] BYE";

}


/**
 * Move the host if the destination is not reached yet. Otherwise
 * calculate a new random position
 */
void GliderMobilityA::makeMove()
{
	debugEV << "start makeMove " << move.info() << endl;

	move.setStart(stepTarget, simTime());

	double ambientAirFlow = 0;

	//todo add rotation
	ambientAirFlow = thermals->getAirFlow(move.getStartPos()).z;		//for now, we only use the z axis
	climbState = climbRate + ambientAirFlow;

	stepTarget.x = (move.getStartPos().x + move.getSpeed() * cos(PI * angle / 180) * SIMTIME_DBL(updateInterval));
	stepTarget.y = (move.getStartPos().y + move.getSpeed() * sin(PI * angle / 180) * SIMTIME_DBL(updateInterval));
	stepTarget.z = (move.getStartPos().z + climbState * SIMTIME_DBL(updateInterval));

	move.setDirectionByTarget(stepTarget);

	debugEV << "new stepTarget: " << stepTarget.info() << endl;

	if(stepTarget.z <= 0)
	{
		EV << "node[" << getParentModule()->getIndex() << "] touched ground. STOPPING" << endl;
		move.setSpeed(0);
	}

	fixIfHostGetsOutside();
}


//todo
void GliderMobilityA::handleSelfMsg(cMessage* msg)
{
	if (msg->getKind() == 31337)
	{
		EV << "node[" << getParentModule()->getIndex() << "] mobility selfMsg" << endl;

		//traceTest << simTime() << "," << targetPos.x << "," << playgroundSizeY() - targetPos.y << "," << targetPos.z << "," << getCurrentPosition().x << "," << playgroundSizeY() - getCurrentPosition().y << ","<< getCurrentPosition().z << endl;

		scheduleAt(simTime() + traceInterval, selfTimer);
	}
	else
	{
		BaseMobility::handleSelfMsg(msg);
	}
}


void GliderMobilityA::initialize(int stage)
{
	BaseMobility::initialize(stage);

	EV << "node[" << getParentModule()->getIndex() << "]:" << " Initializing GliderMobilityA stage " << stage << endl;

	if (stage == 0)
	{

	        move.setSpeed(par("speed").doubleValue());
	        angle = par("angle");
	        angle = fmod(angle,360);
	        climbRate = par("climbRate");
	        secondsPerRotation = par("secondsPerRotation");
	        traceInterval = par("traceInterval");

	}
	else if (stage == 1)
	{
	    	stepTarget = move.getStartPos();

		//Mobility test
		//beginNextMove(new cMessage("self", 0));
		const char *fileTrace = par("traceTest");
		traceTest.open(fileTrace);
		if (!traceTest.is_open())
		{
			EV << "Trace file " << fileTrace << " for glider[" << getParentModule()->getIndex() << "] unable to open for write\n";
			endSimulation();
		}
		traceTest << "simtime,x,y,z,angle,climb" << endl;

		selfTimer = new cMessage("delay-timer", 31337);
		scheduleAt(traceInterval, selfTimer);


		thermals = FindModule<ThermalManager*>::findGlobalModule();
		ASSERT(thermals);

	}
}

void GliderMobilityA::finish()
{
	traceTest.close();
	EV << "node[" << getParentModule()->getIndex() << "] mobility finished" << endl;
}


void GliderMobilityA::fixIfHostGetsOutside()
{
    Coord dummy = Coord::ZERO;
    handleIfOutside(REFLECT, stepTarget, dummy, dummy, angle);
}


//todo
double GliderMobilityA::getDirection(void)
{
	return 0; //nextLoggedPosition.direction-90;				//TODO fade linearly
}

double GliderMobilityA::getPlaygroundArea()
{
	return playgroundSizeX() * playgroundSizeY();
}



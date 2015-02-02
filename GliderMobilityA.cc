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
#include "WindManager.h"
#include "FindModule.h"

Define_Module(GliderMobilityA);

template<typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

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
	static double ambientAirFlowLast = 0.0;

	debugEV << "start makeMove " << move.info() << endl;

	move.setStart(stepTarget, simTime());

	double ambientAirFlow = thermals->getAirFlow(move.getStartPos()).z;		//for now, we only use the z axis
	double stepsize = 3 * SIMTIME_DBL(updateInterval);	//degree per second

	if (ambientAirFlow <= 0)		//thermal lost
	{
		if(abs(turn) > stepsize)
		{
			turn = sgn(turn) * (abs(turn) - stepsize);
		}
		else
		{
			turn = 0;		// at least for now
		}
	}
	else if (turn == 0)		//entering thermal, deciding direction
	{
		Coord leftPos = Coord(move.getStartPos().x + (10 * cos(PI * (angle - 90) / 180)), move.getStartPos().y + (10 * sin(PI * (angle - 90) / 180)), move.getStartPos().z);
		Coord rightPos = Coord(move.getStartPos().x + (10 * cos(PI * (angle + 90) / 180)), move.getStartPos().y + (10 * sin(PI * (angle + 90) / 180)), move.getStartPos().z);
		double left = thermals->getAirFlow(leftPos).z;
		double right = thermals->getAirFlow(rightPos).z;

		if (left < right)
		{
			turn = 0.1;
		}
		else
		{
			turn = -0.1;
		}
	}
	else				//we are thermaling
	{
		double human_factor = uniform(0,1,0);

		if(human_factor <= experience)
		{
			if (ambientAirFlow > ambientAirFlowLast)		//found stronger area, reduce turn
			{
				if (abs(turn) > stepsize)
				{
					turn = sgn(turn) * (abs(turn) - stepsize);
				}
			}
			else if (ambientAirFlow < ambientAirFlowLast)	//found weaker area, increase turn
			{
				//if (abs(turn) < 36)		//10sec turn
				if(ambientAirFlowLast + (-0.0009723 * pow(abs(turn), 2)) > ambientAirFlow + (-0.0009723 * pow(abs(sgn(turn) * (abs(turn) + stepsize)), 2)) )
				{
					turn = sgn(turn) * (abs(turn) + stepsize);
				}

			}
		}
	}



	ambientAirFlowLast = ambientAirFlow;

	climbState = climbRate + ambientAirFlow + (-0.0009723 * pow(abs(turn), 2));

	Coord w = wind->getWind(move.getStartPos());

	angle += turn;								//take increasing downdraft into account

	stepTarget.x = (move.getStartPos().x + (((airSpeed * cos(PI * angle / 180)) + w.x) * SIMTIME_DBL(updateInterval)));
	stepTarget.y = (move.getStartPos().y + (((airSpeed * sin(PI * angle / 180)) + w.y) * SIMTIME_DBL(updateInterval)));
	stepTarget.z = (move.getStartPos().z + climbState * SIMTIME_DBL(updateInterval));

	move.setDirectionByTarget(stepTarget);
	move.setSpeed(stepTarget.distance(move.getStartPos()) / SIMTIME_DBL(updateInterval));

	if (move.getSpeed() == 0)
		move.setSpeed(0.0001);	//workaround in order to avoid end of mobility

	debugEV << "new stepTarget: " << stepTarget.info() << endl;

	if (stepTarget.z <= 0)
	{
		EV << "node[" << getParentModule()->getIndex() << "] touched ground. STOPPING" << endl;
		move.setSpeed(0);
	}

	traceTest << simTime() << "," << move.getStartPos().x << "," << playgroundSizeY() - move.getStartPos().y << "," << move.getStartPos().z << "," << getCourse() << ","
			<< climbState << "," << move.getSpeed() << "," << w.x << "," << w.y << "," << turn << endl;

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
		airSpeed = par("airSpeed").doubleValue();
		move.setSpeed(airSpeed);
		angle = par("angle");
		angle -= 90;
		angle = fmod(angle, 360);
		climbRate = par("climbRate");
		secondsPerRotation = par("secondsPerRotation");
		traceInterval = par("traceInterval");
	        experience = par("experience");

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
		traceTest << "simtime,x,y,z,course,climb,v,windX,windY,turn" << endl;

		selfTimer = new cMessage("delay-timer", 31337);
		scheduleAt(traceInterval, selfTimer);

		thermals = FindModule<ThermalManager*>::findGlobalModule();
		ASSERT(thermals);
		wind = FindModule<WindManager*>::findGlobalModule();
		ASSERT(wind);

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

double GliderMobilityA::getCourse(void)
{
	Coord dir = move.getDirection();
	return fmod((atan2(dir.y, dir.x) * 180.0 / PI) + 90, 360);
}

double GliderMobilityA::getPlaygroundArea()
{
	return playgroundSizeX() * playgroundSizeY();
}


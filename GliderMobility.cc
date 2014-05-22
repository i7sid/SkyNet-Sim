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

#include "Position.h"
#include "LineSegmentsMobilityBase.h"
#include "GliderMobility.h"

Define_Module(GliderMobility);

GliderMobility::GliderMobility()
{

}

GliderMobility::~GliderMobility()
{
	EV << "node[" << getParentModule()->getIndex() << "] BYE";

}

void GliderMobility::evalLog(std::string line)
{
	std::stringstream lineStream(line);
	std::string word;
	int i = 0;        //todo : auto find field position in stream using the index line
	while (getline(lineStream, word, ','))
	{
		switch (i)
		{
		case 0:
			nextLoggedPosition.timeStamp = ((double) atol(word.c_str())) / 1000.0;
			break;
		case 1:
			nextLoggedPosition.x = atof(word.c_str());
			break;
		case 2:
			nextLoggedPosition.y = atof(word.c_str());
			break;
		case 3:
			nextLoggedPosition.z = atof(word.c_str());
			break;
		case 4:
			nextLoggedPosition.direction = atof(word.c_str());
			break;
		case 5:
			nextLoggedPosition.velocity = atof(word.c_str());
			break;
		}
		i++;
	}
}
/*
 void GliderMobility::updateMyPosition()
 {
 std::string line;

 move.setStart(Coord(nextLoggedPosition.x, nextLoggedPosition.y, nextLoggedPosition.altitude), nextLoggedPosition.timeStamp); //we should be there at this time

 EV << "Current position for node[" << getParentModule()->getIndex() << "] x = " << stepTarget.x << " y = " << stepTarget.y << " z = " << stepTarget.z << endl;

 if(!posStream.good())
 {
 EV << "node[" << getParentModule()->getIndex() << "] pos " << "EOF\n";
 nextChange = -1;
 lastSpeed = Coord(0,0,0);
 move.setSpeed(0);
 deleteModule();
 return;
 }


 getline(posStream, line);

 if(line.length() < 1)
 {
 EV << "node[" << getParentModule()->getIndex() << "] pos " << "string too short\n";
 nextChange = -1;
 lastSpeed = Coord(0,0,0);
 move.setSpeed(0);
 deleteModule();
 return;
 }

 evalLog(line);

 nextChange = nextLoggedPosition.timeStamp;

 stepTarget = Coord(nextLoggedPosition.x, nextLoggedPosition.y, nextLoggedPosition.altitude);
 move.setDirectionByTarget(stepTarget);
 move.setSpeed((stepTarget - lastPosition).length() / (nextChange - simTime()).dbl());
 //lastSpeed = (stepTarget - lastPosition) / (nextChange - simTime()).dbl();
 }
 */

void GliderMobility::setTargetPosition()
{
	std::string line;

	EV << "node[" << getParentModule()->getIndex() << "] setTargetPosition\n";

	if (!posStream.good())
	{
		EV << "node[" << getParentModule()->getIndex() << "] pos " << "EOF\n";
		move.setStart(0);       //delete
		return;
	}

	getline(posStream, line);

	if (line.length() < 1)
	{
		EV << "node[" << getParentModule()->getIndex() << "] pos " << "string too short\n";
		move.setSpeed(0);       //delete
		return;
	}

	evalLog(line);

	targetPos.x = nextLoggedPosition.x;
	targetPos.y = nextLoggedPosition.y;
	targetPos.z = nextLoggedPosition.z;
	targetTime = nextLoggedPosition.timeStamp;
	if (targetTime == 0)                             //correct for simtime 0 on first flying node
		targetTime += 0.1;
	if (move.getSpeed() <= 0)                          //circumvent move.getSpeed()==0 nuts...
		move.setSpeed(0.1);

	EV << "targetTime:" << targetTime << " updateInterval: " << updateInterval << endl;

}

void GliderMobility::initialize(int stage)
{
	LineSegmentsMobilityBase::initialize(stage);

	EV << "node[" << getParentModule()->getIndex() << "]:" << " Initializing GliderMobility stage " << stage << endl;

	if (stage == 0)
	{

		const char *file = par("posFile");

		posStream.open(file);
		if (!posStream.is_open())
		{
			EV << "Position file " << file << " for glider[" << getParentModule()->getIndex() << "] not found\n";
			endSimulation();
		}

		std::string line;
		getline(posStream, line);       //skip header
	}
	else if (stage == 1)
	{
		//updateMyPosition();   //skip first position TODO... fix timestemp 0 issue
		//updateMyPosition();

		beginNextMove(new cMessage("self", 0));
	}
}

void GliderMobility::finish()
{
	EV << "node[" << getParentModule()->getIndex() << "] mobility finished" << endl;
}

void GliderMobility::fixIfHostGetsOutside()
{

}

double GliderMobility::getDirection(void)
{
	return nextLoggedPosition.direction;				//TODO fade linearly
}

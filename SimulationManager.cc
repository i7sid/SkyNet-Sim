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
#include <omnetpp.h>


#include <SimulationManager.h>

Define_Module(SimulationManager);

SimulationManager::SimulationManager()
{
	// TODO Auto-generated constructor stub

}

SimulationManager::~SimulationManager()
{
	// TODO Auto-generated destructor stub
}

void SimulationManager::initialize(int stage)
{
	EV << "Initializing SimulationManager stage " << stage << endl;
	if(stage == 0)
	{
		int killTime = par("maxDuration");

		if(killTime)
		{
			EV << "Set end of simulation to: " << killTime << endl;
			scheduleAt(killTime, new cMessage("simEnd-timer", 0));
		}
		else
		{
			EV << "Endless simulation run" << endl;
		}

	}
}

void SimulationManager::handleMessage(cMessage* msg)
{
	    if (!msg->isSelfMessage())
	        error("SimulationManager can only receive self messages");

	if (msg->getKind() == 0)
	{
		delete msg;

		EV << "Desired end of simulation. " << gliders.size() << " gliders still in air...";

		while(gliders.size())
		{
			gliders.back()->callFinish();
			gliders.back()->deleteModule();
			gliders.pop_back();
		}
	}
	else
	{
		BaseModule::handleMessage(msg);
	}
}


void SimulationManager::registerGlider(cModule* parent)
{
	gliders.push_back(parent);
}

void SimulationManager::unregisterGlider(cModule* parent)
{
	if(std::find(gliders.begin(), gliders.end(), parent)!=gliders.end())
	{
		//remove
		gliders.erase(std::find(gliders.begin(), gliders.end(), parent));

	}
	else
	{
		error("glider not found for removal");
	}

	Enter_Method_Silent();

	parent->callFinish();
	parent->deleteModule();
}

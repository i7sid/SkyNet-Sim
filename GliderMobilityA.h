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

#ifndef GLIDERMOBILITYA_H_
#define GLIDERMOBILITYA_H_

#include <fstream>
#include <iostream>

#include "MiXiMDefs.h"
#include "BaseMobility.h"
#include "Position.h"
#include "ThermalManager.h"
#include "WindManager.h"

class MIXIM_API GliderMobilityA :  public BaseMobility
{
  private:

    /** @brief Timer message for scheduling next message.*/
    cMessage *selfTimer;

    double angle;          ///< angle of linear motion
    double climbRate;
    double secondsPerRotation;
    double airSpeed;

    double turn;	//turn in degree / second

    double experience;

    /** @brief always stores the last step for position display update */
    Coord stepTarget;
    double climbState;

    std::ofstream traceTest;
    double traceInterval;

    ThermalManager* thermals;
    WindManager* wind;

  public:
    GliderMobilityA();
    virtual ~GliderMobilityA();

    double getCourse();
    double getPlaygroundArea();

  protected:

    void initialize(int stage);

    void makeMove();

    void fixIfHostGetsOutside();

    void finish();

    void handleSelfMsg(cMessage* msg);
};

#endif /* GLIDERMOBILITYA_H_ */

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

#ifndef GLIDER_H_
#define GLIDER_H_

#include <fstream>
#include <iostream>

#include "MiXiMDefs.h"
#include "LineSegmentsMobilityBase.h"
#include "Position.h"

class MIXIM_API GliderMobility :  public LineSegmentsMobilityBase
{
  private:
    std::ifstream posStream;
    std::ofstream traceTest;

    /** @brief Timer message for scheduling next message.*/
    cMessage *selfTimer;

  public:
    GliderMobility();
    virtual ~GliderMobility();
    double getPlaygroundSizeY(void);

    double getDirection();

  protected:
    Position nextLoggedPosition;

    void setTargetPosition();
    void fixIfHostGetsOutside();

    void initialize(int stage);
    void finish();
    void evalLog(std::string line);

    void handleSelfMsg(cMessage* msg);
};

#endif /* GLIDER_H_ */

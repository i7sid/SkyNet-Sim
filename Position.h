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

#ifndef POSITION_H_
#define POSITION_H_

#include <Coord.h>

class Position : public Coord {

public:
    double timeStamp;
    double direction;
    double velocity;

    Position();
    Position(double timeStamp, double x, double y, double z, double direction, double velocity);
    virtual ~Position();
};

#endif /* POSITION_H_ */

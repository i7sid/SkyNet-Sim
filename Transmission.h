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

#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include <string>

#define PWR_1MW     0
#define PWR_23MW    1
#define PWR_100MW   2
#define PWR_158MW   3
#define PWR_316MW   4

class Transmission
{
public:
    double timeStamp;
    int pwrLevel;
    std::string antenna;
    std::string packetId;

public:
    Transmission(double timeStamp, int pwrLevel, std::string antenna, std::string acketId);
    Transmission();
    virtual ~Transmission();
};

#endif /* TRANSMISSION_H_ */

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

#ifndef THERMAL_H_
#define THERMAL_H_

#include "MiXiMDefs.h"

#include "Coord.h"
#include "FindModule.h"

#define r1r2shape_1	0.1400
#define r1r2shape_2	0.2500
#define r1r2shape_3	0.3600
#define r1r2shape_4	0.4700
#define r1r2shape_5	0.5800
#define r1r2shape_6	0.6900
#define r1r2shape_7	0.8000

#define Kshape_1_1	1.5352
#define Kshape_1_2	2.5826
#define Kshape_1_3	-0.0113
#define Kshape_1_4	-0.1950
#define Kshape_1_5	0.0008

#define Kshape_2_1	1.5265
#define Kshape_2_2	3.6054
#define Kshape_2_3	-0.0176
#define Kshape_2_4	-0.1265
#define Kshape_2_5	0.0005

#define Kshape_3_1	1.4866
#define Kshape_3_2	4.8356
#define Kshape_3_3	-0.0320
#define Kshape_3_4	-0.0818
#define Kshape_3_5	0.0001

#define Kshape_4_1	1.2042
#define Kshape_4_2	7.7904
#define Kshape_4_3	0.0848
#define Kshape_4_4	-0.0445
#define Kshape_4_5	0.0001

#define Kshape_5_1	0.8816
#define Kshape_5_2	13.9720
#define Kshape_5_3	0.3404
#define Kshape_5_4	-0.0216
#define Kshape_5_5	0.0001

#define Kshape_6_1	0.7067
#define Kshape_6_2	23.9940
#define Kshape_6_3	0.5689
#define Kshape_6_4	-0.0099
#define Kshape_6_5	0.0002

#define Kshape_7_1	0.6189
#define Kshape_7_2	42.7965
#define Kshape_7_3	0.7157
#define Kshape_7_4	-0.0033
#define Kshape_7_5	0.0001


class Thermal
{
private:
	Coord pos;
	double base;
	double updraft;
	bool sflag;
	double playgroundArea;

	//could be random
	double wgain; 	//multiplier on vertical velocity
	double rgain; 	//multiplier on radius

public:
	double upDraft(Coord gliderPos);
	Coord positionAtAltitude(double z);

	Thermal(){};

	Thermal(Coord pos, double base, double updraft, bool sflag, double playgroundArea):
	pos(pos), base(base), updraft(updraft), sflag(sflag), playgroundArea(playgroundArea)
	{
		wgain = 1;
		rgain = 1;
	}

	virtual ~Thermal()
	{

	}
};

#endif /* THERMAL_H_ */

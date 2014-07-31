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

#include "Thermal.h"


//Original updraft model by Michael J. Allen, NASA DFRC, 2005 "Updraft Model for Development of Autonomous Soaring Uninhabited Air Vehicles"
//Implemented and extended by Juergen Eckert

Coord Thermal::positionAtAltitude(double z)
{
	return(Coord(pos.x, pos.y, z));	//add wind here

}


double Thermal::upDraft(Coord gliderPos)
{
	//thermal position at gilder altitude
	Coord thermalPos = positionAtAltitude(gliderPos.z);

	//distance thermal center to gilder
	double dist = thermalPos.distance(gliderPos);

	//average updraft size
	double zzi = gliderPos.z / base;
	double rbar = (0.102*pow(zzi,1.0/3.0)) * (1-(0.25*zzi)) * base;

	//average updraft strength
	double wtbar = pow(zzi,1.0/3.0) * (1-1.1*zzi) * updraft;
	return wtbar;

	//inner and outer radius of rotated trapezoid updraft
	double r2 = rbar * rgain; 		//multiply by random perturbation gain
	if(r2<10)				//limit small updrafts to 20m diameter
	    r2 = 10;
	double r1r2;
	if(r2 < 600)
		r1r2 = 0.0011*r2 + 0.14;
	else
		r1r2 = 0.8;

	double r1 = r1r2*r2;

	//multiply average updraft strength by wgain for this updraft
	double wt = wtbar*wgain; 		//add random perturbation

	//strength at center of rotated trapezoid updraft
	double wc = (3 * wt * (pow(r2,3) - pow(r2,2) * r1)) / (pow(r2, 3) - pow(r1,3));

	//updraft velocity												// bad section. can be improved
	double r=dist;
	double rr2 = r/r2;
	double ws, ka, kb, kc, kd;
	if(gliderPos.z < base)				//if you are below cloud base
	{
		if(r1r2 < 0.5 * (r1r2shape_1 + r1r2shape_2))
		{
			ka = Kshape_1_1;
			kb = Kshape_1_2;
			kc = Kshape_1_3;
			kd = Kshape_1_4;
		}
		else if(r1r2 < 0.5 * (r1r2shape_2 + r1r2shape_3))
		{
			ka = Kshape_2_1;
			kb = Kshape_2_2;
			kc = Kshape_2_3;
			kd = Kshape_2_4;
		}
		else if(r1r2 < 0.5 * (r1r2shape_3 + r1r2shape_4))
		{
			ka = Kshape_3_1;
			kb = Kshape_3_2;
			kc = Kshape_3_3;
			kd = Kshape_3_4;
		}
		else if(r1r2 < 0.5 * (r1r2shape_4 + r1r2shape_5))
		{
			ka = Kshape_4_1;
			kb = Kshape_4_2;
			kc = Kshape_4_3;
			kd = Kshape_4_4;
		}
		else if(r1r2 < 0.5 * (r1r2shape_5 + r1r2shape_6))
		{
			ka = Kshape_5_1;
			kb = Kshape_5_2;
			kc = Kshape_5_3;
			kd = Kshape_5_4;
		}
		else if(r1r2 < 0.5 * (r1r2shape_6 + r1r2shape_7))
		{
			ka = Kshape_6_1;
			kb = Kshape_6_2;
			kc = Kshape_6_3;
			kd = Kshape_6_4;
		}
		else
		{
			ka = Kshape_7_1;
			kb = Kshape_7_2;
			kc = Kshape_7_3;
			kd = Kshape_7_4;
		}

		double in = rr2;
		//smooth vertical velocity distribution
		ws = (1.0 / (1+pow(abs(ka*in + kc),kb)) ) + kd*in;			//inconsistent to paper appendix
		if(ws < 0)
			ws=0;					//no neg updrafts
	}
	else
	{
		ws=0;
	}

	// downdraft velocity at the edge of the updraft
	double w1;
	if(dist > r1 && rr2<2)
		w1 = (PI/6) * sin(PI*rr2);					// paper: w1 -= w1;
	else
		w1 = 0;

	double swd;								//scale factor for wd for zzi; used again later
	double wd;
	if(zzi>0.5 && zzi<=0.9)
	{
		swd = 2.5*(zzi-.5);
		wd = swd * w1;
		if(wd > 0)
			wd = 0;
	}
	else
	{
		swd=0;
		wd=0;
	}

	double w2 = ws*wc + wd*wt;

	// environment sink velocity
	double At = PI*pow(rbar,2);							// total area taken by updraft
	ASSERT(At<playgroundArea);							// area of playground is too small

	double we;
	if(sflag)
	{
		we = -(At*wtbar*(1-swd))/(playgroundArea-At);				// environment sink rate, positive up (m/s)
		if(we > 0)								// don't allow positive sink
			we = 0;
	}
	else
	{
		we=0;
	}

	// stretch updraft to blend with sink at edge
	double w;
	if(dist>r1)
		w = w2*(1-we/wc) + we;
	else
		w = w2;

	return w;
}


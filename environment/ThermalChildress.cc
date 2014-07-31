/*
 * ThermalChrildress.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: eckert
 */

#include <ThermalChildress.h>

Coord ThermalChildress::positionAtAltitude(double z)
{
	return(Coord(pos.x, pos.y, z));	//add wind here

}

double ThermalChildress::upDraft(Coord gliderPos)
{
	//thermal position at gilder altitude
	Coord thermalPos = positionAtAltitude(gliderPos.z);

	//distance thermal center to gilder
	double dist = thermalPos.distance(gliderPos);
	double z = gliderPos.z;

	double zstar = z/zi;

	// Thermal Diameter terms
	double dt = zi*(0.4*pow(zstar,1.0/3.0)) * (1-(0.5*zstar)) + ((((zstar-0.6)*zstar*z))/PI);

	// Convection Cell core growth term
	double d1 = 0.17*dt + 0.5*(zstar-0.6)*dt;
	double d2 = dt;
	double r1 = d1/2;
	double r2 = d2/2;

	// vertical velocity terms

	// Deardorff Velocity Scale
	double x = zi * heatflux * (9.86/Ts);
	double wstar = pow(x, 1.0/3.0);

	// Average Vertical Velocity
	double wtavg = wstar * pow(zstar, 1.0/3.0) * (1-(1.1*zstar));

	//Peak Vertical Velocity
	double wpeak = ((3*wtavg)*(pow(r2, 3)-(pow(r2, 2) * r1))) / (pow(r2, 3) - pow(r1, 3));


	//Downdraft Core formation and dimensionless deceleration term
	double wd = (-zi/(1.275 * pow(wstar, 2))) * (0.04/(z/304.8));
	double wcore = (wd*(zstar+0.45)) - (wpeak*0.5);

	// vertical velocity vs. radius plots
	double rt = dt/2;

	double w = 0;
	if(zstar < 1.0)
	{
		if(dist <= rt)
			w = wpeak*cos((dist/rt)*(PI/2));

		switch(downdraftType)
		{
		case NO_DOWNDRAFT:
			break;
		case CENTER_DOWNDRAFT:
		if(zstar > .500  && dist <= r1)
			w += wcore*cos((dist/r1)*(PI/2));
			break;
		case OUTER_DOWNDRAFT:
		if(abs(dist-rt) <= r1)
			w += wcore*cos(((dist-rt)/r1)*(PI/2)) * 0.5;
			break;
		}

	}
    	else if(zstar > 1.0)
    	{
    		w = 0;
    	}
	if(invert)
		return -w;
	else
		return w;
}

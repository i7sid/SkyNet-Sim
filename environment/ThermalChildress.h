/*
 * ThermalChrildress.h
 *
 *  Created on: Jul 21, 2014
 *      Author: eckert
 */

#ifndef THERMALCHILDRESS_H_
#define THERMALCHILDRESS_H_

#include "Coord.h"

enum DDT
{
	NO_DOWNDRAFT,
	CENTER_DOWNDRAFT,			// in 60% of the cases, according to childress
	OUTER_DOWNDRAFT
};

class ThermalChildress
{
private:
	Coord pos;
	double zi;				//CBL height in m
	double Ts;				// Surface Temperature in Deg K	typ: 300.15
	double heatflux;			//Kinematic heat flux (Deg K*m/s). typ: 0.4
	int downdraftType;
	bool invert;

public:
	double upDraft(Coord gliderPos);
	Coord positionAtAltitude(double z);

	ThermalChildress(Coord pos, double zi, double Ts, double heatflux, int downdraftType, bool invert):
	pos(pos), zi(zi), Ts(Ts), heatflux(heatflux), downdraftType(downdraftType), invert(invert)
	{

	};


	ThermalChildress(Coord pos, double zi, int downdraftType):
	pos(pos), zi(zi), downdraftType(downdraftType)
	{
		invert = false;
		heatflux = 0.4;
		Ts = 300.15;
	};

	virtual ~ThermalChildress()
	{
	};

};

#endif /* THERMALCHILDRESS_H_ */

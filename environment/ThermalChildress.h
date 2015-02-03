/*
 * ThermalChrildress.h
 *
 *  Created on: Jul 21, 2014
 *      Author: eckert
 */

#ifndef THERMALCHILDRESS_H_
#define THERMALCHILDRESS_H_

#include "Coord.h"

#include "WindManager.h"

enum DDT
{
	NO_DOWNDRAFT,
	CENTER_DOWNDRAFT,			// in 60% of the cases, according to childress
	OUTER_DOWNDRAFT
};

class MIXIM_API ThermalChildress: public BaseModule
{
private:
	//Coord pos;
	double zi;				//CBL height in m
	double Ts;				// Surface Temperature in Deg K	typ: 300.15
	double heatflux;			//Kinematic heat flux (Deg K*m/s). typ: 0.4
	int downdraftType;
	bool invert;
	WindManager *wind;
	Coord dPos[4000];

public:
	double upDraft(Coord gliderPos);
	Coord positionAtAltitude(double z);
	void updateThermalPos(void);
	void initialize(int stage);

	ThermalChildress();
	virtual ~ThermalChildress()
	{
	};

};

#endif /* THERMALCHILDRESS_H_ */

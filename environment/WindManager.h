/*
 * WindManager.h
 *
 *  Created on: Jul 31, 2014
 *      Author: eckert
 */

#ifndef WINDMANAGER_H_
#define WINDMANAGER_H_

#include "MiXiMDefs.h"
#include "BaseModule.h"
#include "Coord.h"

class MIXIM_API WindManager: public BaseModule
{
private:

        double windDirection;
        double windSpeed;

        double windDirChange;
        double windSpeedChange;

protected:

	void initialize(int stage);

public:

	WindManager();
	virtual ~WindManager();

	Coord getWind(Coord pos);

};


#endif /* WINDMANAGER_H_ */

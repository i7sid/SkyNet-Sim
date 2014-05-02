#include <FWMath.h>
#include <cmath>
#include <math.h>

#include "PgRadioModel.h"
#include "AirFrame_m.h"
#include "../Position.h"
#include "PgPhyLayer.h"
#include "GliderMobility.h"

#define debugEV (ev.isDisabled()||!debug) ? ev : ev << "PhyLayer(PgRadioModel): "

double deg2rad(double deg)
{
	return (deg * PI / 180);
}

double rad2deg(double rad)
{
	return (rad * 180 / PI);
}

double magneticBearing(Coord from, Coord to)
{
	Coord diffC(to - from);
	double bearing = rad2deg(atan2(diffC.y, diffC.x));

	if (bearing < 0)			//just in case
		bearing = bearing + 360;
	if (bearing > 360)
		bearing = bearing - 360;

	return 360 - bearing;
}

double relativSymBearing(double magneticBearing, double heading)
{
	double diff = abs(heading - magneticBearing) % 360;
	if (diff > 180)
		return (360 - diff);
	else
		return (diff);
}

void PgRadioModel::filterSignal(AirFrame *frame, const Coord& senderPos, const Coord& receiverPos)
{
	Signal& sig = frame->getSignal();

/*        if (dynamic_cast<PgPhyLayer*> (sig.getSendingModule()) == NULL)
        	debugEV << "jep" << endl;
        else
        	debugEV << "functioniert" << endl;

        if((dynamic_cast<PgPhyLayer*> (sig.getSendingModule()))->getParentModule()->getParentModule() == NULL)
        	debugEV << "null" << endl;
        else
        	debugEV << "parentname: " << (dynamic_cast<PgPhyLayer*> (sig.getSendingModule()))->getParentModule()->getParentModule()->getName() << endl;
*/
        GliderMobility *gMobTx = (GliderMobility *) (dynamic_cast<PgPhyLayer*> (sig.getSendingModule()))->getParentModule()->getParentModule()->getSubmodule("mobility");
        if(gMobTx == NULL)
        {
        	debugEV << "Sender mobility not found" << endl;
        	ASSERT(false);
        }
        GliderMobility *gMobRx = (GliderMobility *) (dynamic_cast<PgPhyLayer*> (sig.getReceptionModule()))->getParentModule()->getParentModule()->getSubmodule("mobility");
        if(gMobRx == NULL)
        {
        	debugEV << "Receiver mobility not found" << endl;
        	ASSERT(false);
        }

	//helper stuff
	const Coord senderPos2D(senderPos.x, senderPos.y);
	const Coord receiverPos2D(receiverPos.x, receiverPos.y);
	double horDist = senderPos2D.distance(receiverPos2D);
	double deltaZ = abs(senderPos.z - receiverPos.z);

	//main stuff
	double dist = senderPos.distance(receiverPos);
	double alpha = relativSymBearing(magneticBearing(senderPos, receiverPos), gMobTx->getDirection())
			+ relativSymBearing(magneticBearing(receiverPos, senderPos), gMobRx->getDirection());
	double beta = atan(deltaZ / horDist) * 180 / PI;

	debugEV << "Sender position [(x,y,z),dir] = [" << senderPos << "," << gMobTx->getDirection() << "]" << endl;
	debugEV << "Receiver position [(x,y,z),dir] = [" << receiverPos << "," << gMobRx->getDirection() << "]" << endl;

	double lambda = BaseWorldUtility::speedOfLight / carrierFrequency;
	double lossFreeSpace = 20 * log10(4*PI*dist/lambda);
	double lossHor = -kappa * alpha;
	double lossVert = -zeta * beta;

	debugEV << "Add PgRadioModel attenuation parameters (dist,alpha,beta) = (" << dist << "," << alpha << "," << beta << ")" << endl;
	debugEV << "Loss (lossFreeSpace, lossHor, lossVert) = (" << lossFreeSpace << "," << lossHor << "," << lossVert << ")" << endl;

	sig.addAttenuation(new PgRadioModel::Mapping(lossFreeSpace, lossHor, lossVert, debug));
}

DimensionSet PgRadioModel::Mapping::dimensions(DimensionSet::timeFreqDomain);

double PgRadioModel::Mapping::getValue(const Argument& pos) const
{
	double attenuation = lossFreeSpace + lossHor + lossVert;

	debugEV << "Add attenuation (dBm, mW) = (" << attenuation << "," << FWMath::dBm2mW(attenuation) << ")" << endl;

	return 1/FWMath::dBm2mW(attenuation);
}


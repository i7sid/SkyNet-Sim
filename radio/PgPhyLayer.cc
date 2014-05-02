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

#include "PgPhyLayer.h"

#include "PgRadioModel.h"

#include "BaseConnectionManager.h"

Define_Module(PgPhyLayer);

AnalogueModel* PgPhyLayer::getAnalogueModelFromName(std::string name, ParameterMap& params) {

	if (name == "PgRadioModel")
	{
		return initializePgRadioModel(params);
	}

	return PhyLayer::getAnalogueModelFromName(name, params);
}

AnalogueModel* PgPhyLayer::initializePgRadioModel(ParameterMap& params){

	// init with default value
	double carrierFrequency = 8.68e+8;
	double kappa = 0;
	double zeta = 0;
	ParameterMap::iterator it;

	// get carrierFrequency from config
	it = params.find("carrierFrequency");

	if ( it != params.end() ) // parameter carrierFrequency has been specified in config.xml
	{
		// set carrierFrequency
		carrierFrequency = it->second.doubleValue();
		coreEV << "initializePgRadioModel(): carrierFrequency set from config.xml to " << carrierFrequency << endl;

		// check whether carrierFrequency is not smaller than specified in ConnectionManager
		if(cc->hasPar("carrierFrequency") && carrierFrequency < cc->par("carrierFrequency").doubleValue())
		{
			// throw error
			opp_error("PgPhyLayer::initializePgRadioModel(): carrierFrequency can't be smaller than specified in ConnectionManager. Please adjust your config.xml file accordingly");
		}
	}
	else // carrierFrequency has not been specified in config.xml
	{
		if (cc->hasPar("carrierFrequency")) // parameter carrierFrequency has been specified in ConnectionManager
		{
			// set carrierFrequency according to ConnectionManager
			carrierFrequency = cc->par("carrierFrequency").doubleValue();
			coreEV << "PgPhyLayer::initializePgRadioModel(): carrierFrequency set from ConnectionManager to " << carrierFrequency << endl;
		}
		else // carrierFrequency has not been specified in ConnectionManager
		{
			// keep carrierFrequency at default value
			coreEV << "PgPhyLayer::initializePgRadioModel(): carrierFrequency set from default value to " << carrierFrequency << endl;
		}
	}

	// get carrierFrequency from config
	it = params.find("kappa");

	if ( it != params.end() ) // parameter carrierFrequency has been specified in config.xml
	{
		// set carrierFrequency
		kappa = it->second.doubleValue();
		coreEV << "initializePgRadioModel(): kappa set from config.xml to " << kappa << endl;

		// check whether carrierFrequency is not smaller than specified in ConnectionManager
		if(cc->hasPar("kappa") && carrierFrequency < cc->par("kappa").doubleValue())
		{
			// throw error
			opp_error("PgPhyLayer::initializePgRadioModel(): kappa can't be smaller than specified in ConnectionManager. Please adjust your config.xml file accordingly");
		}
	}
	else // kappa has not been specified in config.xml
	{
		if (cc->hasPar("kappa")) // parameter kappa has been specified in ConnectionManager
		{
			// set kappa according to ConnectionManager
			kappa = cc->par("kappa").doubleValue();
			coreEV << "PgPhyLayer::createPathLossModel(): kappa set from ConnectionManager to " << kappa << endl;
		}
		else // kappa has not been specified in ConnectionManager
		{
			// keep kappa at default value
			coreEV << "PgPhyLayer::createPathLossModel(): kappa set from default value to " << kappa << endl;
		}
	}

	// get carrierFrequency from config
	it = params.find("zeta");

	if ( it != params.end() ) // parameter zeta has been specified in config.xml
	{
		// set zeta
		zeta = it->second.doubleValue();
		coreEV << "initializePgRadioModel(): zeta set from config.xml to " << zeta << endl;

		// check whether zeta is not smaller than specified in ConnectionManager
		if(cc->hasPar("zeta") && zeta < cc->par("zeta").doubleValue())
		{
			// throw error
			opp_error("PgPhyLayer::initializePgRadioModel(): zeta can't be smaller than specified in ConnectionManager. Please adjust your config.xml file accordingly");
		}
	}
	else // zeta has not been specified in config.xml
	{
		if (cc->hasPar("zeta")) // parameter zeta has been specified in ConnectionManager
		{
			// set zeta according to ConnectionManager
			zeta = cc->par("zeta").doubleValue();
			coreEV << "PgPhyLayer::initializePgRadioModel(): zeta set from ConnectionManager to " << zeta << endl;
		}
		else // zeta has not been specified in ConnectionManager
		{
			// keep zeta at default value
			coreEV << "PgPhyLayer::initializePgRadioModel(): zeta set from default value to " << zeta << endl;
		}
	}

	return new PgRadioModel(carrierFrequency, kappa, zeta, coreDebug);

}

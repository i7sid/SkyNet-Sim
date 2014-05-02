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

#ifndef PGPHYLAYER_H_
#define PGPHYLAYER_H_

#include "MiXiMDefs.h"
#include "PhyLayer.h"

class MIXIM_API PgPhyLayer : public PhyLayer
{
protected:

	/**
	 * @brief Creates and returns an instance of the AnalogueModel with the
	 * specified name.
	 *
	 * Is able to initialize the following AnalogueModels:
	 * - PgRadioModel
	 */
	virtual AnalogueModel* getAnalogueModelFromName(std::string name, ParameterMap& params);

	AnalogueModel* initializePgRadioModel(ParameterMap& params);

};

#endif /* PGPHYLAYER_H_ */

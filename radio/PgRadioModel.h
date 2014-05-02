#ifndef ANALOGUEMODEL_PGRADIOMODEL_H
#define ANALOGUEMODEL_PGRADIOMODEL_H

#include "AnalogueModel.h"
#include "BaseWorldUtility.h"
#include "MappingBase.h"

class PgRadioModel: public AnalogueModel
{

public:
	PgRadioModel(double carrierFrequency, double kappa, double zeta, bool debug) :
		carrierFrequency(carrierFrequency), kappa(kappa), zeta(zeta), debug(debug)
	{
	}

	virtual ~PgRadioModel()
	{
	}

	virtual void filterSignal(AirFrame *frame, const Coord& sendersPos, const Coord& receiverPos);

protected:

	class Mapping: public SimpleConstMapping
	{
	protected:
		static DimensionSet dimensions;

		double lossFreeSpace;
		double lossHor;
		double lossVert;
		bool debug;
	public:
		Mapping(double lossFreeSpace, double lossHor, double lossVert, bool debug) :
				SimpleConstMapping(dimensions), lossFreeSpace(lossFreeSpace), lossHor(lossHor), lossVert(lossVert), debug(debug)
		{
		}

		virtual double getValue(const Argument& pos) const;

		ConstMapping* constClone() const
		{
			return new Mapping(*this);
		}
	};

	 double carrierFrequency;
	 double kappa;
	 double zeta;


	/** @brief Whether debug messages should be displayed. */
	bool debug;
};

#endif /* ANALOGUEMODEL_PGRADIOMODEL_H */

#ifndef HAMMURABI_MODEL_H_
#define HAMMURABI_MODEL_H_

#include "Order.h"
#include "Statistics.h"
#include "GameConstants.h"

struct Model {
public:
	Order order;
	Statistics info;

	Model(int acres, int population, int bushels)
		: order()
		, info(acres, population, bushels)
	{}

	int AvaliableBushels() {
		return info.bushels_in_stocks 
			- order.bushels_to_eat 
			+ order.acres_for_trade * info.acre_cost
			- order.acres_to_sow * GameConstants::kBushelsToSowAcre;
	}

	int AvaliableAcres() {
		return info.acres + order.acres_for_trade - order.acres_to_sow;
	}
};

#endif // HAMMURABI_MODEL_H_

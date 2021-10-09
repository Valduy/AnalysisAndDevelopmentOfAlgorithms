#include "Model.h"

Model::Model() : Model(0, 0, 0) {}

Model::Model(int acres, int population, int bushels)
	: order()
	, info(acres, population, bushels)
{}

int Model::AvaliableBushels() {
	return info.bushels_in_stocks
		- order.bushels_to_eat
		- order.acres_for_trade * info.acre_cost
		- order.acres_to_sow * GameConstants::kBushelsToSowAcre;
}

int Model::AvaliableAcres() {
	return info.acres + order.acres_for_trade - order.acres_to_sow;
}

void Model::Serialize(std::ostream& os) {
	order.Serialize(os);
	info.Serialize(os);
}

void Model::Deserialize(std::istream& is) {
	order.Deserialize(is);
	info.Deserialize(is);
}
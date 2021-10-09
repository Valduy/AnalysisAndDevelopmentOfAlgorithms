#ifndef HAMMURABI_MODEL_H_
#define HAMMURABI_MODEL_H_

#include "Order.h"
#include "Statistics.h"
#include "GameConstants.h"

struct Model : public ISerializable{
public:
	Order order;
	Statistics info;

	Model();
	Model(int acres, int population, int bushels);

	int AvaliableBushels();
	int AvaliableAcres();

	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
};

#endif // HAMMURABI_MODEL_H_

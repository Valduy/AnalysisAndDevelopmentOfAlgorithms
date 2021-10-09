#ifndef	HAMMURABI_ORDER_H_
#define HAMMURABI_ORDER_H_

#include "ISerializable.h"

class Order : public ISerializable{
public:
	int acres_for_trade;
	int bushels_to_eat;
	int acres_to_sow;

	Order();

	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
};

#endif // HAMMURABI_ORDER_H_
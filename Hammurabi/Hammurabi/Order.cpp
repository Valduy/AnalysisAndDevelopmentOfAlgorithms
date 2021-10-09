#include "Order.h"

Order::Order()
	: acres_for_trade(0)
	, bushels_to_eat(0)
	, acres_to_sow(0)
{}

void Order::Serialize(std::ostream& os) {
	os << acres_for_trade << "\n";
	os << bushels_to_eat << "\n";
	os << acres_to_sow << "\n";
}

void Order::Deserialize(std::istream& is) {
	is >> acres_for_trade;
	is >> bushels_to_eat;
	is >> acres_to_sow;
}
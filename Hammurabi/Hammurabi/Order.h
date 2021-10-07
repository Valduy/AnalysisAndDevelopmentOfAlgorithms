#ifndef	HAMMURABI_ORDER_H_
#define HAMMURABI_ORDER_H_

struct Order {
	int acres_for_trade;
	int bushels_to_eat;
	int acres_to_sow;

	Order() 
		: acres_for_trade(0)
		, bushels_to_eat(0)
		, acres_to_sow(0)
	{}
};

#endif // HAMMURABI_ORDER_H_
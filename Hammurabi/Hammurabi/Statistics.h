#ifndef HAMMURABI_STATISTICS_H_
#define HAMMURABI_STATISTICS_H_

struct Statistics {
public:
	int year;
	int acres;
	int acre_cost;
	int population;
	int hunger_deaths;
	int newcomers;
	int bushels_per_acre;
	int harvested_bushels;
	int bushels_in_stocks;
	int rat_damage;
	bool is_plagued;

	Statistics(int acres, int population, int bushels)
		: year(1)
		, acres(acres)
		, acre_cost(0)
		, population(population)
		, hunger_deaths(0)
		, newcomers(0)
		, bushels_per_acre(0)
		, harvested_bushels(0)
		, bushels_in_stocks(bushels)
		, rat_damage(0)
		, is_plagued(false)
	{}
};

#endif // HAMMURABI_STATISTICS_H_
#ifndef HAMMURABI_STATISTICS_H_
#define HAMMURABI_STATISTICS_H_

#include <vector>
#include "ISerializable.h"

struct Statistics : public ISerializable {
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
	std::vector<int> hunger_deaths_percents;

	Statistics(int acres, int population, int bushels);

	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
};

#endif // HAMMURABI_STATISTICS_H_
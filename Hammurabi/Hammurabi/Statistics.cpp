#include "Statistics.h"

Statistics::Statistics(int acres, int population, int bushels)
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
	, hunger_deaths_percents()
{}

void Statistics::Serialize(std::ostream& os) {
	os << year << "\n";
	os << acres << "\n";
	os << acre_cost << "\n";
	os << population << "\n";
	os << hunger_deaths << "\n";
	os << newcomers << "\n";
	os << bushels_per_acre << "\n";
	os << harvested_bushels << "\n";
	os << bushels_in_stocks << "\n";
	os << rat_damage << "\n";
	os << is_plagued << "\n";
	
	for (int hunger_deaths_percent : hunger_deaths_percents) {
		os << hunger_deaths_percent << " ";
	}
	os << "\n";
}

void Statistics::Deserialize(std::istream & is) {
	is >> year;
	is >> acres;
	is >> acre_cost;
	is >> population;
	is >> hunger_deaths;
	is >> newcomers;
	is >> bushels_per_acre;
	is >> harvested_bushels;
	is >> bushels_in_stocks;
	is >> rat_damage;
	is >> is_plagued;

	int hunger_deaths_percent;

	for (int i = 0; i < year; ++i) {
		is >> hunger_deaths_percent;
		hunger_deaths_percents.push_back(hunger_deaths_percent);
	}
}

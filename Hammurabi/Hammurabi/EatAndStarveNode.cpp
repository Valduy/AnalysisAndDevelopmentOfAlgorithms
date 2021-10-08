#include <iostream>
#include "EatAndStarveNode.h"

bool EatAndStarveNode::Act(Model& model) {
	model.info.bushels_in_stocks -= model.order.bushels_to_eat;
	int potential_fed = model.order.bushels_to_eat / GameConstants::kManYearBushelsConsumption;

	if (model.info.population > potential_fed) {
		model.info.hunger_deaths = model.info.population - potential_fed;
		int prev_population = model.info.population;
		model.info.population = potential_fed; 
		model.info.hunger_deaths_percents.push_back(model.info.hunger_deaths * 100 / prev_population);


		if (model.info.hunger_deaths >= prev_population * GameConstants::kMaxHungerDeathsPercent / 100) {
			std::cout << "�� �������� ������� " << model.info.hunger_deaths << "!!!\n"
				"��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������.\n"
				"������ �� ��������� ������� ������ ������������� � ��������.\n";
			return false;
		}
	}

	return true;
}

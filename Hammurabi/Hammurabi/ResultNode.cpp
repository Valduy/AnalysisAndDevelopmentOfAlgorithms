#include <numeric>
#include <iostream>
#include "ResultNode.h"

bool ResultNode::Act(Model& model) {
	if (model.info.year <= GameConstants::kRoundsCount) return true;

	int hunger_percent_avg = std::accumulate(
		model.info.hunger_deaths_percents.begin(),
		model.info.hunger_deaths_percents.end(), 0.0) / model.info.year;

	int acres_per_man = model.info.acres / model.info.population;

	if (hunger_percent_avg > 33 && acres_per_man < 7) {
		std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.\n"
			"Теперь вы вынуждены влачить жалкое существование в изгнании.\n";
	}
	else if (hunger_percent_avg > 10 && acres_per_man < 9) {
		std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному.\n"
			"Народ вздохнул с облегчением и никто больше не желает видеть вас привителемы.\n";
	}
	else if (hunger_percent_avg > 3 && acres_per_man < 10) {
		std::cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы\n"
			"увидеть вас во главе города снова.\n";
	}
	else {
		std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше!\n";
	}

	return false;
}

#include "BriefingNode.h"

bool BriefingNode::Act(Model& model) {
	std::cout << "ћой ѕовелитель, соизволь поведать тебе\n"
		"\tв году " << model.info.year << " твоего правлени€:\n";

	if (model.info.hunger_deaths > 0) {
		std::cout << "\t" << model.info.hunger_deaths << " человек умерли с голоду;\n";
	}
	if (model.info.newcomers > 0) {
		std::cout << "\t" << model.info.newcomers << " человек прибыли в наш великий город;\n";
	}
	if (model.info.is_plagued) {
		std::cout << "\t" << "чума уничтожила половину населени€;\n";
	}

	std::cout << "\t" << "население города сейчас составл€ет " << model.info.population << " человек;\n";

	if (model.info.harvested_bushels > 0) {
		std::cout << "\t" << "мы собрали " << model.info.harvested_bushels << " бушелей пшеницы, по "
			<< model.info.bushels_per_acre << " бушел€ с акра;\n";
	}
	if (model.info.rat_damage > 0) {
		std::cout << "\t" << "крысы истребили " << model.info.rat_damage << " бушелей пшеницы\n";
	}

	std::cout << "\t" << "¬ закромах города осталось " << model.info.bushels_in_stocks << " бушелей пшеницы.\n";
	std::cout << "\t" << "город сейчас занимает " << model.info.acres << " акров;\n";
	std::cout << "\t" << "1 акр земли стоит сейчас " << model.info.acre_cost << " бушель.\n";
	std::cout << "„то пожелаешь повелить?\n";
	return true;
}
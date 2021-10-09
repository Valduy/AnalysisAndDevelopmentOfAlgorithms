#include "WheatManagmentNode.h"

bool WheatManagmentNode::Act(Model& model) {
	std::cout << "Сколько акров земли прикажете засеять?\n";
	int acres = 0;

	do {
		std::cin >> acres;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "О, Повелитель, смилуйтесь над моим скудным разумением, "
				"я не смогу сосчитать столько акров...\n";
			continue;
		}

		std::string error_message;

		if (IsValid(model, acres, error_message)) {
			break;
		}
		else {
			std::cout << error_message;
		}
	} while (true);

	model.order.acres_to_sow = acres;
	return true;
}

bool WheatManagmentNode::IsValid(Model& model, int acres, std::string& error_message) {
	if (model.info.population * GameConstants::kAcresPerMan < acres) {
		error_message = "Но Повелитель, нам не хватит людей чтобы засеять столько акров...\n";
		return false;
	}
	if (model.AvaliableBushels() < acres / GameConstants::kAcresSowedByBushel) {
		error_message = "Но Повелитель, у нас недостаточно зерна, чтобы засеять столько акров..\n";
		return false;
	}
	if (model.AvaliableAcres() < acres) {
		error_message = "Но Повелитель, у нас нет столько земли...\n";
		return false;
	}

	return true;
}
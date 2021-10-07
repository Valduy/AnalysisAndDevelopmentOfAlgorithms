#include "FoodManagmentNode.h"

bool FoodManagmentNode::Act(Model& model) {
	std::cout << "Сколько бушелей пшеницы повелеваете съесть?\n";
	int bushels = 0;

	do {
		std::cin >> bushels;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "О, Повелитель, смилуйтесь над моим скудным разумением, "
				"я не смогу сосчитать столько бушелей...\n";
			continue;
		}

		std::string error_message;

		if (IsValid(model, bushels, error_message)) {
			break;
		}
		else {
			std::cout << error_message;
		}
	} while (true);

	model.order.bushels_to_eat = bushels;
	return true;
}

bool FoodManagmentNode::IsValid(Model& model, int bushels, std::string& error_message) {
	if (bushels > model.AvaliableBushels()) {
		error_message = "Но Повелитель, у нас нет столько пшеницы...\n";
		return false;
	}
	if (bushels < 0) {
		error_message = "Но Повелитель, вы же не хотите заморить нас голодом, правда?..\n";
		return false;
	}

	return true;
}
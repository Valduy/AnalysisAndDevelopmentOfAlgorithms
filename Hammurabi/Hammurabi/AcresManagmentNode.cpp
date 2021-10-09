#include "AcresManagmentNode.h"

bool AcresManagmentNode::Act(Model& model) {
	std::cout << "Как пожелаете распорядиться нашими землями, Повелитель?\n";
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

	model.order.acres_for_trade = acres;
	return true;
}

bool AcresManagmentNode::IsValid(Model& model, int acres, std::string& error_message) {
	int trade_cost = acres * model.info.acre_cost;

	// Если: int ovrflow.
	if (acres != 0 && trade_cost / acres != model.info.acre_cost) {
		error_message = "Но Повелитель, столько акров попросту не существует...\n";
		return false;
	}
	// Если: не хватает бушелей.
	if (trade_cost > model.AvaliableBushels()) {
		error_message = "Но Повелитель, мы не можем себе этого позволить.\n"
			"В наших амбарах лишь только " + std::to_string(model.AvaliableAcres()) + " бушелей...\n";
		return false;
	}
	// Если: не хватает акров.
	if ((acres < 0) && (model.AvaliableAcres() + acres < 0)) {
		error_message = "Но Повелитель, у нас нет столько земель.\n"
			"Наши владения занимают лишь только " + std::to_string(model.AvaliableAcres()) + " акров...\n";
		return false;
	}

	return true;
}
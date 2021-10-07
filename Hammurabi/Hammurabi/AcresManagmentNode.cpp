#include "AcresManagmentNode.h"

bool AcresManagmentNode::Act(Model& model) {
	std::cout << " ак пожелаете распор€дитьс€ нашими земл€ми, повелитель?\n";
	int acres = 0;

	do {
		std::cin >> acres;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "ќ, ѕовелитель, смилуйтесь над моим скудным разумением, "
				"€ не смогу сосчитать столько акров...\n";
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

bool AcresManagmentNode::IsValid(Model& model, int acres, std::string& error) {
	if (acres * model.info.acre_cost > model.AvaliableBushels()) {
		error = "Ќо повелитель, мы не можем себе этого позволить...\n";
		return false;
	}
	if ((acres < 0) && (model.AvaliableAcres() + acres < 0)) {
		error = "Ќо повелитель, у нас нет столько земель...\n";
		return false;
	}

	return true;
}
#include "WheatManagmentNode.h"

WheatManagmentNode::WheatManagmentNode(Messanger& messanger)
	: messanger_(messanger)
{}

bool WheatManagmentNode::Act(Model& model) {
	std::string error_message;
	int acres = messanger_.RequestValue<int>(question_, error_notification_);

	while (!IsValid(model, acres, error_message)) {
		std::cout << error_message;
		acres = messanger_.RequestValue<int>(question_, error_notification_);
	}

	model.order.acres_to_sow = acres;
	return true;
}

bool WheatManagmentNode::IsValid(Model& model, int acres, std::string& error_message) {
	// Если: введено отрицательное значение.
	if (acres < 0) {
		error_message = "Но Повелитель, как это понимать?..\n";
		return false;
	}
	// Если: не хватает акров.
	if (model.AvaliableAcres() < acres) {
		error_message = "Но Повелитель, у нас нет столько земель.\n"
			"Наши владения занимают лишь только " + std::to_string(model.AvaliableAcres()) + " акров...\n";
		return false;
	}
	// Если: не хватает людей.
	if (model.info.population * GameConstants::kAcresPerMan < acres) {
		error_message = "Но Повелитель, нам не хватит людей чтобы засеять столько акров.\n"
			"Ныне в городе живет только " + std::to_string(model.info.population) + " человек...\n";
		return false;
	}
	// Если: не хватает бушелей.
	if (model.AvaliableBushels() < acres / GameConstants::kAcresSowedByBushel) {
		error_message = "Но Повелитель, у нас недостаточно зерна, чтобы засеять столько акров.\n"
			"В наших амбарах лишь только " + std::to_string(model.AvaliableBushels()) + " бушелей...\n";
		return false;
	}

	return true;
}
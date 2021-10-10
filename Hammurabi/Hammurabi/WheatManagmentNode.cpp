#include <sstream>
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
	std::stringstream sstream;

	// Если: введено отрицательное значение.
	if (acres < 0) {
		error_message = "Но Повелитель, как это понимать?..\n";
		return false;
	}
	// Если: не хватает акров.
	if (model.AvaliableAcres() < acres) {
		sstream << "Но Повелитель, у нас нет столько земель.\nНаши владения занимают лишь только "
			<< model.AvaliableAcres() << " акров...\n";
		error_message = sstream.str();
		return false;
	}
	// Если: не хватает людей.
	if (model.info.population * GameConstants::kAcresPerMan < acres) {
		sstream << "Но Повелитель, нам не хватит людей чтобы засеять столько акров.\nНыне в городе живет только "
			<< model.info.population << " человек...\n";
		error_message = sstream.str();
		return false;
	}
	// Если: не хватает бушелей.
	if (model.AvaliableBushels() < acres / GameConstants::kAcresSowedByBushel) {
		sstream << "Но Повелитель, у нас недостаточно зерна, чтобы засеять столько акров.\nВ наших амбарах лишь только "
			<< model.AvaliableBushels() << " бушелей...\n";
		error_message = sstream.str();
		return false;
	}

	return true;
}
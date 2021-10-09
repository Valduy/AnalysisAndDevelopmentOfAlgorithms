#include <string>
#include "FoodManagmentNode.h"

FoodManagmentNode::FoodManagmentNode(Messanger& messanger)
	: messanger_(messanger)
{}

bool FoodManagmentNode::Act(Model& model) {
	std::string error_message;
	int bushels = messanger_.RequestValue<int>(question_, error_notification_);

	while (!IsValid(model, bushels, error_message)) {
		std::cout << error_message;
		bushels = messanger_.RequestValue<int>(question_, error_notification_);
	}

	model.order.bushels_to_eat = bushels;
	return true;
}

bool FoodManagmentNode::IsValid(Model& model, int bushels, std::string& error_message) {
	// Если: не хватает бушелей.
	if (bushels > model.AvaliableBushels()) {
		error_message = "Но Повелитель, у нас нет столько пшеницы.\n"
			"В наших амбарах лишь только " + std::to_string(model.AvaliableBushels()) + " бушелей...\n";
		return false;
	}
	// Если: введено отрицательное значение.
	if (bushels < 0) {
		error_message = "Но Повелитель, вы же не хотите заморить нас голодом, правда?..\n";
		return false;
	}

	return true;
}
#include <string>
#include <sstream>
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
	std::stringstream sstream;
	
	// ����: �� ������� �������.
	if (bushels > model.AvaliableBushels()) {
		sstream << "�� ����������, � ��� ��� ������� �������.\n� ����� ������� ���� ������ "
			<< model.AvaliableBushels() << " �������...\n";
		error_message = sstream.str();
		return false;
	}
	// ����: ������� ������������� ��������.
	if (bushels < 0) {
		error_message = "�� ����������, �� �� �� ������ �������� ��� �������, ������?..\n";
		return false;
	}

	return true;
}
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

	// ����: ������� ������������� ��������.
	if (acres < 0) {
		error_message = "�� ����������, ��� ��� ��������?..\n";
		return false;
	}
	// ����: �� ������� �����.
	if (model.AvaliableAcres() < acres) {
		sstream << "�� ����������, � ��� ��� ������� ������.\n���� �������� �������� ���� ������ "
			<< model.AvaliableAcres() << " �����...\n";
		error_message = sstream.str();
		return false;
	}
	// ����: �� ������� �����.
	if (model.info.population * GameConstants::kAcresPerMan < acres) {
		sstream << "�� ����������, ��� �� ������ ����� ����� ������� ������� �����.\n���� � ������ ����� ������ "
			<< model.info.population << " �������...\n";
		error_message = sstream.str();
		return false;
	}
	// ����: �� ������� �������.
	if (model.AvaliableBushels() < acres / GameConstants::kAcresSowedByBushel) {
		sstream << "�� ����������, � ��� ������������ �����, ����� ������� ������� �����.\n� ����� ������� ���� ������ "
			<< model.AvaliableBushels() << " �������...\n";
		error_message = sstream.str();
		return false;
	}

	return true;
}
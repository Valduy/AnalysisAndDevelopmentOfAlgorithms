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
	// ����: ������� ������������� ��������.
	if (acres < 0) {
		error_message = "�� ����������, ��� ��� ��������?..\n";
		return false;
	}
	// ����: �� ������� �����.
	if (model.AvaliableAcres() < acres) {
		error_message = "�� ����������, � ��� ��� ������� ������.\n"
			"���� �������� �������� ���� ������ " + std::to_string(model.AvaliableAcres()) + " �����...\n";
		return false;
	}
	// ����: �� ������� �����.
	if (model.info.population * GameConstants::kAcresPerMan < acres) {
		error_message = "�� ����������, ��� �� ������ ����� ����� ������� ������� �����.\n"
			"���� � ������ ����� ������ " + std::to_string(model.info.population) + " �������...\n";
		return false;
	}
	// ����: �� ������� �������.
	if (model.AvaliableBushels() < acres / GameConstants::kAcresSowedByBushel) {
		error_message = "�� ����������, � ��� ������������ �����, ����� ������� ������� �����.\n"
			"� ����� ������� ���� ������ " + std::to_string(model.AvaliableBushels()) + " �������...\n";
		return false;
	}

	return true;
}
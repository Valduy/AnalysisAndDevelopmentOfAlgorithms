#include "WheatManagmentNode.h"

bool WheatManagmentNode::Act(Model& model) {
	std::cout << "������� ����� ����� ��������� �������?\n";
	int acres = 0;

	do {
		std::cin >> acres;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "�, ����������, ���������� ��� ���� ������� ����������, "
				"� �� ����� ��������� ������� �����...\n";
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
		error_message = "�� ����������, ��� �� ������ ����� ����� ������� ������� �����...\n";
		return false;
	}
	if (model.AvaliableBushels() < acres * GameConstants::kBushelsToSowAcre) {
		error_message = "�� ����������, � ��� ������������ �����, ����� ������� ������� �����..\n";
		return false;
	}
	if (model.AvaliableAcres() < acres) {
		error_message = "�� ����������, � ��� ��� ������� �����...\n";
		return false;
	}

	return true;
}
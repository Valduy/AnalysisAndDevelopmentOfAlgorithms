#include "AcresManagmentNode.h"

bool AcresManagmentNode::Act(Model& model) {
	std::cout << "��� ��������� ������������� ������ �������, ����������?\n";
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

	model.order.acres_for_trade = acres;
	return true;
}

bool AcresManagmentNode::IsValid(Model& model, int acres, std::string& error_message) {
	int trade_cost = acres * model.info.acre_cost;

	// ����: int ovrflow.
	if (acres != 0 && trade_cost / acres != model.info.acre_cost) {
		error_message = "�� ����������, ������� ����� �������� �� ����������...\n";
		return false;
	}
	// ����: �� ������� �������.
	if (trade_cost > model.AvaliableBushels()) {
		error_message = "�� ����������, �� �� ����� ���� ����� ���������.\n"
			"� ����� ������� ���� ������ " + std::to_string(model.AvaliableAcres()) + " �������...\n";
		return false;
	}
	// ����: �� ������� �����.
	if ((acres < 0) && (model.AvaliableAcres() + acres < 0)) {
		error_message = "�� ����������, � ��� ��� ������� ������.\n"
			"���� �������� �������� ���� ������ " + std::to_string(model.AvaliableAcres()) + " �����...\n";
		return false;
	}

	return true;
}
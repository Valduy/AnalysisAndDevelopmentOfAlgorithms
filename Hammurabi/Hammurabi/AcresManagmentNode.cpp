#include <sstream>
#include "AcresManagmentNode.h"

AcresManagmentNode::AcresManagmentNode(Messanger& messanger)
	: messanger_(messanger)
{}

bool AcresManagmentNode::Act(Model& model) {
	std::string error_message;
	int acres = messanger_.RequestValue<int>(question_, error_notification_);

	while (!IsValid(model, acres, error_message)) {
		std::cout << error_message;
		acres = messanger_.RequestValue<int>(question_, error_notification_);
	}

	model.order.acres_for_trade = acres;
	return true;
}

bool AcresManagmentNode::IsValid(Model& model, int acres, std::string& error_message) {
	std::stringstream sstream;
	int trade_cost = acres * model.info.acre_cost;

	// ����: int ovrflow.
	if (acres != 0 && trade_cost / acres != model.info.acre_cost) {
		error_message = "�� ����������, ������� ����� �������� �� ����������...\n";
		return false;
	}
	// ����: �� ������� �������.
	if (trade_cost > model.AvaliableBushels()) {
		sstream << "�� ����������, �� �� ����� ���� ����� ���������.\n� ����� ������� ���� ������ "
			<< model.AvaliableAcres() << " �������...\n";
		error_message = sstream.str();
		return false;
	}
	// ����: �� ������� �����.
	if ((acres < 0) && (model.AvaliableAcres() + acres < 0)) {
		sstream << "�� ����������, � ��� ��� ������� ������.\n���� �������� �������� ���� ������ "
			<< model.AvaliableAcres() << " �����...\n";
		error_message = sstream.str();
		return false;
	}

	return true;
}
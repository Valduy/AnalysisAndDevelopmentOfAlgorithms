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

	// Если: int ovrflow.
	if (acres != 0 && trade_cost / acres != model.info.acre_cost) {
		error_message = "Но Повелитель, столько акров попросту не существует...\n";
		return false;
	}
	// Если: не хватает бушелей.
	if (trade_cost > model.AvaliableBushels()) {
		sstream << "Но Повелитель, мы не можем себе этого позволить.\nВ наших амбарах лишь только "
			<< model.AvaliableAcres() << " бушелей...\n";
		error_message = sstream.str();
		return false;
	}
	// Если: не хватает акров.
	if ((acres < 0) && (model.AvaliableAcres() + acres < 0)) {
		sstream << "Но Повелитель, у нас нет столько земель.\nНаши владения занимают лишь только "
			<< model.AvaliableAcres() << " акров...\n";
		error_message = sstream.str();
		return false;
	}

	return true;
}
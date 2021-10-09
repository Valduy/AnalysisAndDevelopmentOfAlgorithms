#ifndef HAMMURABI_FOODMANAGMENTNODE_H_
#define HAMMURABI_FOODMANAGMENTNODE_H_

#include <iostream>
#include "IActionNode.h"
#include "Messanger.h"

class FoodManagmentNode : public IActionNode {
public:
	FoodManagmentNode(Messanger& messanger);

	bool Act(Model& model) override;

private:
	const std::string question_ = "Сколько бушелей пшеницы повелеваете съесть?\n";
	const std::string error_notification_ = "О, Повелитель, смилуйтесь над моим скудным разумением, "
		"я не смогу сосчитать столько бушелей...\n";

	Messanger& messanger_;	

	bool IsValid(Model& model, int bushels, std::string& error_message);
};

#endif // HAMMURABI_FOODMANAGMENTNODE_H_
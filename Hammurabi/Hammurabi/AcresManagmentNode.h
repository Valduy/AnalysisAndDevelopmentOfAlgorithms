#ifndef HAMMURABI_ACREBUYNODE_H_
#define HAMMURABI_ACREBUYNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"
#include "Messanger.h"

class AcresManagmentNode : public IActionNode {
public:
	AcresManagmentNode(Messanger& messanger);

	bool Act(Model& model) override;

private:
	const std::string question_ = "Как пожелаете распорядиться нашими землями, Повелитель?\n";
	const std::string error_notification_ = "О, Повелитель, смилуйтесь над моим скудным разумением, "
		"я не смогу сосчитать столько акров...\n";

	Messanger& messanger_;	

	bool IsValid(Model& model, int acres, std::string& error);
};

#endif // HAMMURABI_ACREBUYNODE_H_
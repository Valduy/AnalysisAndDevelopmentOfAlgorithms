#ifndef HAMMURABI_FOODMANAGMENTNODE_H_
#define HAMMURABI_FOODMANAGMENTNODE_H_

#include <iostream>
#include "IActionNode.h"

class FoodManagmentNode : public IActionNode {
public:
	bool Act(Model& model) override;

private:
	bool IsValid(Model& model, int bushels, std::string& error_message);
};

#endif // HAMMURABI_FOODMANAGMENTNODE_H_
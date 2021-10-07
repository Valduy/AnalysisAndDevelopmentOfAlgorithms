#ifndef HAMMURABI_WHEATMANAGMENTNODE_H_
#define HAMMURABI_WHEATMANAGMENTNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"

class WheatManagmentNode : public IActionNode {
public:
	bool Act(Model& model) override;

private:
	bool IsValid(Model& model, int acres, std::string& error_message);
};

#endif // HAMMURABI_WHEATMANAGMENTNODE_H_

#ifndef HAMMURABI_ACREBUYNODE_H_
#define HAMMURABI_ACREBUYNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"

class AcresManagmentNode : public IActionNode {
public:
	bool Act(Model& model) override;

private:
	bool IsValid(Model& model, int acres, std::string& error);
};

#endif // HAMMURABI_ACREBUYNODE_H_
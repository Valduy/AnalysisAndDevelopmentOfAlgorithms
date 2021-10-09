#ifndef HAMMURABI_STOPNODE_H_
#define HAMMURABI_STOPNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"

class StopNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_STOPNODE_H_
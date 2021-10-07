#ifndef HAMMURABI_BRIEFINGNODE_H_
#define HAMMURABI_BRIEFINGNODE_H_

#include <iostream>
#include "IActionNode.h"

class BriefingNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_BRIEFINGNODE_H_
#ifndef HAMMURABI_EATANDSTARVENODE_H_
#define HAMMURABI_EATANDSTARVENODE_H_

#include "IActionNode.h"

class EatAndStarveNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_EATANDSTARVENODE_H_
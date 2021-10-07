#ifndef HAMMURABI_NEWCOMERSNODE_H_
#define HAMMURABI_NEWCOMERSNODE_H_

#include "IActionNode.h"

class NewcomersNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_NEWCOMERSNODE_H_
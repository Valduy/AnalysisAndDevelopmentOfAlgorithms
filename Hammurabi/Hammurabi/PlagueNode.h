#ifndef HAMMURABI_PLAGUENODE_H_
#define HAMMURABI_PLAGUENODE_H_

#include "IActionNode.h"
#include "Random.h"

class PlagueNode : public IActionNode {
public:
	PlagueNode();

	bool Act(Model& model) override;

private:
	Random random_;
};

#endif // HAMMURABI_PLAGUENODE_H_
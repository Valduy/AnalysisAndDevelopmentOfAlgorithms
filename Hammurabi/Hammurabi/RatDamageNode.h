#ifndef HAMMURABI_RATDAMAGENODE_H_
#define HAMMURABI_RATDAMAGENODE_H_

#include "IActionNode.h"
#include "Random.h"

class RatDamageNode : public IActionNode {
public:
	RatDamageNode();

	bool Act(Model& model) override;

private:
	Random random_;
};

#endif // HAMMURABI_RATDAMAGENODE_H_
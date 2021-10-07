#ifndef HAMMURABI_SOWANDREAPNODE_H_
#define HAMMURABI_SOWANDREAPNODE_H_

#include "IActionNode.h"
#include "Random.h"

class SowAndReapNode : public IActionNode {
public:
	SowAndReapNode();

	bool Act(Model& model) override;

private:
	Random random_;
};

#endif // HAMMURABI_SOWANDREAPNODE_H_
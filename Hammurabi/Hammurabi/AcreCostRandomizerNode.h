#ifndef HAMMURABI_ACRECOSTRANDOMIZERNODE_H_
#define HAMMURABI_ACRECOSTRANDOMIZERNODE_H_

#include "IActionNode.h"
#include "Random.h"

class AcreCostRandomizerNode : public IActionNode {
public:
	AcreCostRandomizerNode();

	bool Act(Model& model) override;

private:
	Random random_;
};

#endif // HAMMURABI_ACRECOSTRANDOMIZERNODE_H_

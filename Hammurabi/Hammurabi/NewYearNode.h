#ifndef HAMMURABI_NEWYEARNODE_H_
#define HAMMURABI_NEWYEARNODE_H_

#include "IActionNode.h"

class NewYearNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_NEWYEARNODE_H_
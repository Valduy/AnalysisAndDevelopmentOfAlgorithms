#ifndef HAMMURABI_RESULTNODE_H_
#define HAMMURABI_RESULTNODE_H_

#include "IActionNode.h"

class ResultNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_RESULTNODE_H_
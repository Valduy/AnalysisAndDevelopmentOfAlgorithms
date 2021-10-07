#ifndef HAMMURABI_RESETORDERNODE_H_
#endif HAMMURABI_RESETORDERNODE_H_

#include "IActionNode.h"

class ResetOrderNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#define HAMMURABI_RESETORDERNODE_H_
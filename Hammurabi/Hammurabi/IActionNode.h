#ifndef HAMMURABI_IACTIONNODE_H_
#define HAMMURABI_IACTIONNODE_H_

#include "Model.h"

class IActionNode {
public:
	virtual bool Act(Model& model) = 0;
};

#endif // HAMMURABI_IACTIONNODE_H_

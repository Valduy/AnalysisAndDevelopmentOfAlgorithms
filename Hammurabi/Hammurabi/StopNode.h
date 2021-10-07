#ifndef HAMMURABI_STOPNODE_H_
#define HAMMURABI_STOPNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"

class StopNode : public IActionNode {
public:
	bool Act(Model& model) override;

private:
	const std::string kYesAnswer = "1";
	const std::string kNoAnswer = "0";
};

#endif // HAMMURABI_STOPNODE_H_
#ifndef HAMMURABI_STOPNODE_H_
#define HAMMURABI_STOPNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"
#include "Messanger.h"

class StopNode : public IActionNode {
public:
	StopNode(Messanger& messanger);

	bool Act(Model& model) override;

private:
	std::string question_ = "Не желаете ли прерваться?\n";
	std::string error_notification_ = "О, Повелитель, смилуйтесь над моим скудным разумением, "
		"я не знаю, что мне нужно сделать...\n";

	Messanger& messanger_;	
};

#endif // HAMMURABI_STOPNODE_H_
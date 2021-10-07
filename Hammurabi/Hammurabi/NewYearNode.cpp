#include "NewYearNode.h"

bool NewYearNode::Act(Model& model) {
	model.info.year++;
	return true;
}

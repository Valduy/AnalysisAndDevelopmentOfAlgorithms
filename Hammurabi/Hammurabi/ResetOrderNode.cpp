#include "ResetOrderNode.h"

bool ResetOrderNode::Act(Model& model) {
	model.order.acres_for_trade = 0;
	model.order.bushels_to_eat = 0;
	model.order.acres_to_sow = 0;
	return true;
}

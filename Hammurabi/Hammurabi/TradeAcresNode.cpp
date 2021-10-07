#include "TradeAcresNode.h"

bool TradeAcresNode::Act(Model& model) {
	model.info.acres += model.order.acres_for_trade;
	model.info.bushels_in_stocks -= model.order.acres_for_trade * model.info.acre_cost;
	return true;
}
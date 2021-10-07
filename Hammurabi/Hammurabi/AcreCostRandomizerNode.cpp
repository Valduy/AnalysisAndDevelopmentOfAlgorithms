#include "AcreCostRandomizerNode.h"

AcreCostRandomizerNode::AcreCostRandomizerNode()
	: random_(GameConstants::kMinAcreCost, GameConstants::kMaxAcreCost)
{}

bool AcreCostRandomizerNode::Act(Model& model) {
	model.info.acre_cost = random_.Next();
	return true;
}
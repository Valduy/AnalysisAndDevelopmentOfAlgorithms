#include "SowAndReapNode.h"

SowAndReapNode::SowAndReapNode() 
	: random_(GameConstants::kMinBushelsPerAcre, GameConstants::kMaxBushelsPerAcre)
{}

bool SowAndReapNode::Act(Model& model) {
	model.info.bushels_in_stocks -= model.order.acres_to_sow / GameConstants::kAcresSowedByBushel;
	model.info.bushels_per_acre = random_.Next();
	model.info.harvested_bushels = model.order.acres_to_sow * model.info.bushels_per_acre;
	model.info.bushels_in_stocks += model.info.harvested_bushels;
	return true;
}
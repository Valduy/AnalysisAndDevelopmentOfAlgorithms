#include "RatDamageNode.h"

RatDamageNode::RatDamageNode() 
	: random_(GameConstants::kMinRatDamagePercent, GameConstants::kMaxRatDamagePercent)
{}

bool RatDamageNode::Act(Model& model) {
	model.info.rat_damage = model.info.bushels_in_stocks * random_.Next() / 100;
	model.info.bushels_in_stocks -= model.info.rat_damage;
	return true;
}

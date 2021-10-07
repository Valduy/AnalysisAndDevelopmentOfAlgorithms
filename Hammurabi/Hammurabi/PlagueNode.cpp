#include "PlagueNode.h"

PlagueNode::PlagueNode() 
	: random_(0, 100)
{}

bool PlagueNode::Act(Model& model) {
	if (random_.Next() <= GameConstants::kPlagueChance) {
		model.info.is_plagued = true;
		model.info.population /= 2;
	}

	return true;
}

#include "PlagueNode.h"

PlagueNode::PlagueNode() 
	: random_(0, 100)
{}

bool PlagueNode::Act(Model& model) {
	model.info.is_plagued = random_.Next() <= GameConstants::kPlagueChance;

	if (model.info.is_plagued) {
		model.info.population /= 2;
	}

	return true;
}

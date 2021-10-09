#include "ActionsChain.h"

ActionsChain::ActionsChain(Model& model)
	: model_(model)
	, current_(0)
{}

ActionsChain::~ActionsChain() {
	for (auto it : nodes_) {
		delete it;
	}

	nodes_.clear();
}

void ActionsChain::Run() {
	while (GetCurrent()->Act(model_)) {
		MoveNext();
	}
}

IActionNode* ActionsChain::GetCurrent() {
	return nodes_[current_];
}

void ActionsChain::MoveNext() {
	current_ += 1;

	if (current_ >= nodes_.size()) {
		current_ = 0;
	}
}
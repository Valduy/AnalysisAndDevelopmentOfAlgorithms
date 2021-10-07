#include "ActionsChain.h"

ActionsChain::ActionsChain(Model& model)
	: model_(model)
	, current_(0)
{}

ActionsChain& ActionsChain::Add(IActionNode& node) {
	nodes_.push_back(&node);
	return *this;
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
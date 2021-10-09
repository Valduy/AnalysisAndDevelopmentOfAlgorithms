#ifndef HAMMURABI_ACTIONSCHAIN_H_
#define HAMMURABI_ACTIONSCHAIN_H_

#include <vector>
#include <functional>
#include "IActionNode.h"

class ActionsChain {
public:
	ActionsChain(Model& model);
	~ActionsChain();

	template<typename TActionNode>
	ActionsChain& Add() {
		nodes_.push_back(new TActionNode());
		return *this;
	}

	template<typename TActionNode>
	ActionsChain& Add(std::function<TActionNode*()> factory) {
		nodes_.push_back(factory());
		return *this;
	}

	void Run();

private:
	Model &model_;
	std::vector<IActionNode*> nodes_;
	size_t current_;

	IActionNode* GetCurrent();
	void MoveNext();
};

#endif // HAMMURABI_ACTIONSCHAIN_H_

#ifndef HAMMURABI_TRADEACRESNODE_H_
#define HAMMURABI_TRADEACRESNODE_H_

#include "IActionNode.h"

class TradeAcresNode : public IActionNode {
public:
	bool Act(Model& model) override;
};

#endif // HAMMURABI_TRADEACRESNODE_H_
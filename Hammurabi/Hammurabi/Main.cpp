#include <iostream>
#include "Model.h"
#include "ResetOrderNode.h"
#include "AcreCostRandomizerNode.h"
#include "BriefingNode.h"
#include "AcresManagmentNode.h"
#include "FoodManagmentNode.h"
#include "WheatManagmentNode.h"
#include "TradeAcresNode.h"
#include "EatAndStarveNode.h"
#include "SowAndReapNode.h"
#include "NewcomersNode.h"
#include "RatDamageNode.h"
#include "PlagueNode.h"
#include "NewYearNode.h"
#include "StopNode.h"
#include "ActionsChain.h"

int main() {
	std::setlocale(LC_ALL, "Russian");
	std::cout << "Добро пожаловать в \"Хаммурапи - правитель Вавилона\"!\n";

	Model model(1000, 100, 2800);
	
	ResetOrderNode reset_order_node;
	AcreCostRandomizerNode acre_cost_randomizer_node;
	BriefingNode briefing_node;
	AcresManagmentNode acres_buy_node;
	FoodManagmentNode food_managment_node;
	WheatManagmentNode wheat_managment_node;
	TradeAcresNode trade_acres_node;
	EatAndStarveNode eat_and_starve_node;
	SowAndReapNode sow_and_reap_node;
	NewcomersNode newcomers_node;
	RatDamageNode rat_damage_node;
	PlagueNode plague_node;
	NewYearNode new_year_node;
	StopNode stop_node;

	ActionsChain game(model);
	game
		.Add(reset_order_node)
		.Add(acre_cost_randomizer_node)
		.Add(briefing_node)
		.Add(acres_buy_node)
		.Add(food_managment_node)
		.Add(wheat_managment_node)
		.Add(trade_acres_node)
		.Add(eat_and_starve_node)
		.Add(sow_and_reap_node)
		.Add(newcomers_node)
		.Add(rat_damage_node)
		.Add(plague_node)
		.Add(new_year_node)
		.Add(stop_node);

	game.Run();
	return 0;
}
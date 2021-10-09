#include <iostream>
#include <fstream>
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
#include "ResultNode.h"
#include "StopNode.h"
#include "ActionsChain.h"

int main() {
	std::setlocale(LC_ALL, "Russian");
	std::cout << "Добро пожаловать в \"Хаммурапи - правитель Вавилона\"!\n";

	std::ifstream ifs(GameConstants::kSaveFileName, std::ios_base::in);
	Model model(1000, 100, 2800);

	if (!ifs.fail()) {		
		std::string answer;

		do {
			std::cout << "Хотите продолжить с последнего сохранения? (Да: "
				<< GameConstants::kYesAnswer << "/Нет: " << GameConstants::kNoAnswer << ")\n";
			std::cin >> answer;

			if (answer.compare(GameConstants::kYesAnswer) == 0 || answer.compare(GameConstants::kNoAnswer) == 0) {
				break;
			}

			std::cout << "О, Повелитель, смилуйтесь над моим скудным разумением, "
				"я не знаю, что мне нужно сделать...\n";
		} while (true);

		if (answer.compare(GameConstants::kYesAnswer) == 0) {
			model.Deserialize(ifs);
		}
	}	

	ifs.close();

	ActionsChain game(model);
	game.Add<ResetOrderNode>()
		.Add<AcreCostRandomizerNode>()
		.Add<BriefingNode>()
		.Add<AcresManagmentNode>()
		.Add<FoodManagmentNode>()
		.Add<WheatManagmentNode>()
		.Add<TradeAcresNode>()
		.Add<EatAndStarveNode>()
		.Add<SowAndReapNode>()
		.Add<NewcomersNode>()
		.Add<RatDamageNode>()
		.Add<PlagueNode>()
		.Add<NewYearNode>()
		.Add<ResultNode>()
		.Add<StopNode>();

	game.Run();
	return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include "Messanger.h"
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

void TryToLoadSave(Model& model, Messanger& messanger) {
	std::ifstream ifs(GameConstants::kSaveFileName, std::ios_base::in);

	if (!ifs.fail()) {
		std::string question = "������ ���������� � ���������� ����������?\n";
		std::string error_notification = "�, ����������, ���������� ��� ���� ������� ����������, "
			"� �� ����, ��� ��� ����� �������...\n";

		if (messanger.RequestYesOrNo(question, error_notification)) {
			model.Deserialize(ifs);
		}
	}

	ifs.close();
}

int main() {
	std::setlocale(LC_ALL, "Russian");
	std::cout << "����� ���������� � \"��������� - ��������� ��������\"!\n";
		
	Messanger messanger;
	Model model(1000, 100, 2800);
	TryToLoadSave(model, messanger);

	ActionsChain game(model);
	game.Add<ResetOrderNode>()
		.Add<AcreCostRandomizerNode>()
		.Add<BriefingNode>()
		.Add<AcresManagmentNode>([&messanger]() { return new AcresManagmentNode(messanger); })
		.Add<FoodManagmentNode>([&messanger]() { return new FoodManagmentNode(messanger); })
		.Add<WheatManagmentNode>([&messanger]() { return new WheatManagmentNode(messanger); })
		.Add<TradeAcresNode>()
		.Add<EatAndStarveNode>()
		.Add<SowAndReapNode>()
		.Add<NewcomersNode>()
		.Add<RatDamageNode>()
		.Add<PlagueNode>()
		.Add<NewYearNode>()
		.Add<ResultNode>()
		.Add<StopNode>([&messanger]() { return new StopNode(messanger); });

	game.Run();
	return 0;
}
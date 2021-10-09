#include <fstream>
#include "StopNode.h"

bool StopNode::Act(Model& model) {
	std::string answer;

	do {
		std::cout << "Не желаете ли прерваться ? (Да: " 
			<< GameConstants::kYesAnswer << "/Нет: " << GameConstants::kNoAnswer << ")\n";
		std::cin >> answer;

		if (answer.compare(GameConstants::kYesAnswer) == 0 || answer.compare(GameConstants::kNoAnswer) == 0) {
			break;
		}

		std::cout << "О, Повелитель, смилуйтесь над моим скудным разумением, "
			"я не знаю, что мне нужно сделать...\n";
	} while (true);

	if (answer.compare(GameConstants::kYesAnswer) == 0) {
		std::ofstream ofs(GameConstants::kSaveFileName, std::ios_base::out | std::ios_base::trunc);
		model.Serialize(ofs);
		ofs.flush();
		ofs.close();
		return false;
	}
	else {
		return true;
	}
}
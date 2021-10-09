#include <fstream>
#include "StopNode.h"

bool StopNode::Act(Model& model) {
	std::string answer;

	do {
		std::cout << "Не желаете ли прерваться? (" << kYesAnswer << "/" << kNoAnswer << ")\n";
		std::cin >> answer;
	} while (answer.compare(kYesAnswer) != 0 && answer.compare(kNoAnswer) != 0);

	std::cout << "\n";

	if (answer.compare(kYesAnswer) == 0) {
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
#include "StopNode.h"

bool StopNode::Act(Model& model) {
	std::string answer;

	do {
		std::cout << "Ќе желаете ли прерватьс€? (" << kYesAnswer << "/" << kNoAnswer << ")\n";
		std::cin >> answer;
		auto t1 = answer.compare(kYesAnswer);
		auto t2 = answer.compare(kNoAnswer);
		auto r = t1 == t2;
	} while (answer.compare(kYesAnswer) != 0 && answer.compare(kNoAnswer) != 0);

	std::cout << "\n";

	if (answer.compare(kYesAnswer) == 0) {
		// TODO: сериализаци€
		return false;
	}
	else {
		return true;
	}
}
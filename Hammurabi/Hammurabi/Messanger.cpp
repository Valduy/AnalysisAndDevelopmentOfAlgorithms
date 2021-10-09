#include <iostream>
#include "Messanger.h"
#include "GameConstants.h"

bool Messanger::RequestYesOrNo(const std::string& question, const std::string& error_notification) {
	std::string answer;

	do {
		std::cout << question;
		std::cin >> answer;

		if (answer.compare(GameConstants::kYesAnswer) == 0) {
			return true;
		}
		if (answer.compare(GameConstants::kNoAnswer) == 0) {
			return false;
		}

		std::cout << error_notification;
	} while (true);
}
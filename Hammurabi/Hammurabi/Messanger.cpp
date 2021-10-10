#include <iostream>
#include "Messanger.h"
#include "GameConstants.h"

bool Messanger::RequestYesOrNo(const std::string& question, const std::string& error_notification) {
	std::string answer;

	do {
		std::cout << question;
		std::cout << "(Да: " << kYesAnswer << "/Нет: " << kNoAnswer << ")\n";
		std::cin >> answer;

		if (answer.compare(kYesAnswer) == 0) {
			return true;
		}
		if (answer.compare(kNoAnswer) == 0) {
			return false;
		}

		std::cout << error_notification;
	} while (true);
}
#ifndef HAMMURABI_MESSANGER_H_
#define HAMMURABI_MESSANGER_H_

#include <string>

class Messanger {
public:
	bool RequestYesOrNo(const std::string& question, const std::string& error_notification);

	template<typename T>
	T RequestValue(const std::string& question, const std::string& error_notification) {
		T result;

		do {
			std::cout << question;
			std::cin >> result;

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << error_notification;
			}
			else {
				return result;
			}
		} while (true);
	}
};

#endif // HAMMURABI_MESSANGER_H_
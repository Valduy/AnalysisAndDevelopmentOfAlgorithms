#ifndef HAMMURABI_WHEATMANAGMENTNODE_H_
#define HAMMURABI_WHEATMANAGMENTNODE_H_

#include <iostream>
#include <string>
#include "IActionNode.h"
#include "Messanger.h"

class WheatManagmentNode : public IActionNode {
public:
	WheatManagmentNode(Messanger& messanger);

	bool Act(Model& model) override;

private:
	const std::string question_ = "������� ����� ����� ��������� �������?\n";
	const std::string error_notification_ = "�, ����������, ���������� ��� ���� ������� ����������, "
		"� �� ����� ��������� ������� �����...\n";

	Messanger& messanger_;

	bool IsValid(Model& model, int acres, std::string& error_message);
};

#endif // HAMMURABI_WHEATMANAGMENTNODE_H_

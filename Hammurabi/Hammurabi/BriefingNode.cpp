#include "BriefingNode.h"

bool BriefingNode::Act(Model& model) {
	std::cout << "��� ����������, �������� �������� ����\n"
		"\t� ���� " << model.info.year << " ������ ���������:\n";

	if (model.info.hunger_deaths > 0) {
		std::cout << "\t" << model.info.hunger_deaths << " ������� ������ � ������;\n";
	}
	if (model.info.newcomers > 0) {
		std::cout << "\t" << model.info.newcomers << " ������� ������� � ��� ������� �����;\n";
	}
	if (model.info.is_plagued) {
		std::cout << "\t" << "���� ���������� �������� ���������;\n";
	}

	std::cout << "\t" << "��������� ������ ������ ���������� " << model.info.population << " �������;\n";

	if (model.info.harvested_bushels > 0) {
		std::cout << "\t" << "�� ������� " << model.info.harvested_bushels << " ������� �������, �� "
			<< model.info.bushels_per_acre << " ������ � ����;\n";
	}
	if (model.info.rat_damage > 0) {
		std::cout << "\t" << "����� ��������� " << model.info.rat_damage << " ������� �������\n";
	}

	std::cout << "\t" << "� �������� ������ �������� " << model.info.bushels_in_stocks << " ������� �������.\n";
	std::cout << "\t" << "����� ������ �������� " << model.info.acres << " �����;\n";
	std::cout << "\t" << "1 ��� ����� ����� ������ " << model.info.acre_cost << " ������.\n";
	std::cout << "��� ��������� ��������?\n";
	return true;
}
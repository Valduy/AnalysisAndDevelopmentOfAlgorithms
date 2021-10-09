#include <numeric>
#include <iostream>
#include "ResultNode.h"

bool ResultNode::Act(Model& model) {
	if (model.info.year <= GameConstants::kRoundsCount) return true;

	int hunger_percent_avg = std::accumulate(
		model.info.hunger_deaths_percents.begin(),
		model.info.hunger_deaths_percents.end(), 0) / model.info.year;

	int acres_per_man = model.info.acres / model.info.population;

	if (hunger_percent_avg > 33 && acres_per_man < 7) {
		std::cout << "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������.\n"
			"������ �� ��������� ������� ������ ������������� � ��������.\n";
	}
	else if (hunger_percent_avg > 10 && acres_per_man < 9) {
		std::cout << "�� ������� �������� �����, ������� ������ � ����� ��������.\n"
			"����� �������� � ����������� � ����� ������ �� ������ ������ ��� �����������.\n";
	}
	else if (hunger_percent_avg > 3 && acres_per_man < 10) {
		std::cout << "�� ���������� ������ �������, � ���, �������, ���� ���������������, �� ������ ������ ��\n"
			"������� ��� �� ����� ������ �����.\n";
	}
	else {
		std::cout << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����!\n";
	}

	return false;
}

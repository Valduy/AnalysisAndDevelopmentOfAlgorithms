#ifndef HAMMURABI_GAMECONSTANTS_H_
#define HAMMURABI_GAMECONSTANTS_H_

// �����, ���������� �������� ������� ���������.
class GameConstants {
public:
	// ����������� ���� ����.
	static constexpr int kMinAcreCost = 17; 
	// ������������ ���� ����.
	static constexpr int kMaxAcreCost = 26; 
	// ����������� ���������� ������� � ����.
	static constexpr int kMinBushelsPerAcre = 1; 
	// ������������ ���������� ������� � ����.
	static constexpr int kMaxBushelsPerAcre = 6; 
	// ������� ����� �������, ����� ������� ���� ���.
	static constexpr float kBushelsToSowAcre = 0.5f;
	// ������� ����� ����� ���������� ���� �������.
	static constexpr int kAcresPerMan = 10;
	// ������� ������� ���������� 1 ������� �� ���.
	static constexpr int kManYearBushelsConsumption = 20;
	// ������� ������� �� ������, ������������ ��������.
	static constexpr int kMaxHungerDeathsPercent = 45;
	// ����������� ������� ������, ��������� �������.
	static constexpr int kMinRatDamagePercent = 0;
	// ������������ ������� ������, ��������� �������.
	static constexpr int kMaxRatDamagePercent = 7;
	// ����������� ����.
	static constexpr int kPlagueChance = 15;
	// ����������� ���������� �������������.
	static constexpr int kMinNewcomers = 0;
	// ������������ ���������� �������������.
	static constexpr int kMaxNewcomers = 50;
	// ���������� ������� �������.
	static constexpr int kRoundsCount = 10;
};

#endif // HAMMURABI_GAMECONSTANTS_H_

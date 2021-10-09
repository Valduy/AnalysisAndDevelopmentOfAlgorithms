#ifndef HAMMURABI_GAMECONSTANTS_H_
#define HAMMURABI_GAMECONSTANTS_H_

// Класс, содержащий основные игровые константы.
class GameConstants {
public:
	// Минимальная цена акра.
	static constexpr int kMinAcreCost = 17; 
	// Максимальная цена акра.
	static constexpr int kMaxAcreCost = 26; 
	// Минимальное количество бушелей с акра.
	static constexpr int kMinBushelsPerAcre = 1; 
	// Максимальное количество бушелей с акра.
	static constexpr int kMaxBushelsPerAcre = 6; 
	// Сколько акров можно засеять одним бушелем.
	static constexpr int kAcresSowedByBushel = 2;
	// Сколько акров может обработать один человек.
	static constexpr int kAcresPerMan = 10;
	// Сколько бушелей потребляет 1 человек за год.
	static constexpr int kManYearBushelsConsumption = 20;
	// Процент смертей от голода, определяющий проигрыш.
	static constexpr int kMaxHungerDeathsPercent = 45;
	// Минимальный процент ущерба, наносимый крысами.
	static constexpr int kMinRatDamagePercent = 0;
	// Максимальный процент ущерба, наносимый крысами.
	static constexpr int kMaxRatDamagePercent = 7;
	// Вероятность чумы.
	static constexpr int kPlagueChance = 15;
	// Минимальное количество новоприбывших.
	static constexpr int kMinNewcomers = 0;
	// Максимальное количество новоприбывших.
	static constexpr int kMaxNewcomers = 50;
	// Количество игровых раундов.
	static constexpr int kRoundsCount = 10;
	// Имя файла с сохранением.
	static constexpr const char* kSaveFileName = "Save.txt";
	// Ответ да.
	static constexpr const char* kYesAnswer = "1";
	// Ответ нет.
	static constexpr const char* kNoAnswer = "0";
};

#endif // HAMMURABI_GAMECONSTANTS_H_

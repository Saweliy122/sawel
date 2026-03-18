#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <codecvt> 

#ifdef _WIN32
#include <windows.h>
#endif
std::wstring utf8_to_utf16(const std::string& utf8) {
    if (utf8.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0,
        utf8.c_str(), (int)utf8.size(), NULL, 0);
    std::wstring utf16(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0,
        utf8.c_str(), (int)utf8.size(), &utf16[0], size_needed);
    return utf16;
}

class Weather {
private:
    std::string date;
    std::string windDirection;
    double temperature;
    double humidity;
    double pressure;
    std::string dayOfWeek;

public:
    // Конструктор по умолчанию
    Weather() : temperature(0.0), humidity(0.0), pressure(0.0) {}

    // Конструктор с параметрами
    Weather(std::string d, std::string wind, double temp, double hum, double pres, std::string day)
        : date(d), windDirection(wind), temperature(temp), humidity(hum), pressure(pres), dayOfWeek(day) {}

    // Геттеры
    std::string getDate() const { return date; }
    std::string getWindDirection() const { return windDirection; }
    double getTemperature() const { return temperature; }
    double getHumidity() const { return humidity; }
    double getPressure() const { return pressure; }
    std::string getDayOfWeek() const { return dayOfWeek; }

    // Метод для преобразования в JSON строку
    std::string toJSON() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << "    {\n";
        ss << "      \"dayOfWeek\": \"" << dayOfWeek << "\",\n";
        ss << "      \"date\": \"" << date << "\",\n";
        ss << "      \"windDirection\": \"" << windDirection << "\",\n";
        ss << "      \"temperature\": " << temperature << ",\n";
        ss << "      \"humidity\": " << humidity << ",\n";
        ss << "      \"pressure\": " << pressure << "\n";
        ss << "    }";
        return ss.str();
    }

    
    void display() const {
        std::cout << "┌─────────────────────────────────┐\n";
        std::cout << "│ " << std::left << std::setw(15) << dayOfWeek << " │ " << std::setw(10) << date << " │\n";
        std::cout << "├─────────────────────────────────┤\n";
        std::cout << "│ Ветер: " << std::left << std::setw(20) << windDirection << "│\n";
        std::cout << "│ Температура: " << std::setw(16) << std::to_string(temperature) + "°C" << "│\n";
        std::cout << "│ Влажность: " << std::setw(18) << std::to_string(humidity) + "%" << "│\n";
        std::cout << "│ Давление: " << std::setw(18) << std::to_string(pressure) + " мм рт.ст." << "│\n";
        std::cout << "└─────────────────────────────────┘\n";
    }
};

class WeeklyWeather {
private:
    std::vector<Weather> weekWeather;
    std::string filename;
    std::vector<std::string> dayNames = {
        "Понедельник", "Вторник", "Среда", "Четверг",
        "Пятница", "Суббота", "Воскресенье"
    };

    
    void initializeTestData() {
        
        weekWeather.clear();

        
        weekWeather.push_back(Weather("26.02.2026", "Northern", -5.5, 85.0, 750.0, "Monday"));
        weekWeather.push_back(Weather("27.02.2026", "Northeast", -3.2, 78.0, 752.5, "Tuesday"));
        weekWeather.push_back(Weather("28.02.2026", "Eastern", -1.8, 72.0, 755.0, "Wednesday"));
        weekWeather.push_back(Weather("01.03.2026", "South-east", 0.5, 70.0, 753.2, "Thursday"));
        weekWeather.push_back(Weather("02.03.2026", "South", 2.3, 75.0, 751.8, "Friday"));
        weekWeather.push_back(Weather("03.03.2026", "Southwest", 1.5, 80.0, 749.5, "Saturday"));
        weekWeather.push_back(Weather("04.03.2026", "Western", -0.7, 82.0, 748.0, "Sunday"));
    }

public:
    WeeklyWeather(const std::string& fname) : filename(fname) {
        
        initializeTestData();
    }

    
    bool saveToFile() {
        if (weekWeather.empty()) {
            std::cout << "Нет данных для сохранения!\n";
            return false;
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла для записи!" << std::endl;
            return false;
        }

        file << "{\n";
        file << "  \"weeklyWeather\": [\n";

        for (size_t i = 0; i < weekWeather.size(); ++i) {
            file << weekWeather[i].toJSON();
            if (i < weekWeather.size() - 1) {
                file << ",";
            }
            file << "\n";
        }

        file << "  ],\n";

        
        file << "  \"statistics\": {\n";
        file << "    \"averageTemperature\": " << calculateAverageTemperature() << ",\n";
        file << "    \"maxTemperature\": " << findMaxTemperature() << ",\n";
        file << "    \"minTemperature\": " << findMinTemperature() << ",\n";
        file << "    \"averageHumidity\": " << calculateAverageHumidity() << ",\n";
        file << "    \"averagePressure\": " << calculateAveragePressure() << "\n";
        file << "  }\n";
        file << "}\n";

        file.close();
        std::cout << "\n✓ Данные успешно сохранены в файл " << filename << std::endl;
        return true;
    }

    
    void displayWeeklyData() const {
        if (weekWeather.empty()) {
            std::cout << "\nНет данных о погоде.\n";
            return;
        }

        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║     ПРОГНОЗ ПОГОДЫ НА НЕДЕЛЮ         ║\n";
        std::cout << "╚════════════════════════════════════════╝\n\n";

        for (size_t i = 0; i < weekWeather.size(); ++i) {
            weekWeather[i].display();
        }

        
        std::cout << "\n📊 СТАТИСТИКА ЗА НЕДЕЛЮ:\n";
        std::cout << "──────────────────────────\n";
        std::cout << "Средняя температура: " << std::fixed << std::setprecision(1)
            << calculateAverageTemperature() << "°C\n";
        std::cout << "Максимальная температура: " << findMaxTemperature() << "°C\n";
        std::cout << "Минимальная температура: " << findMinTemperature() << "°C\n";
        std::cout << "Средняя влажность: " << calculateAverageHumidity() << "%\n";
        std::cout << "Среднее давление: " << calculateAveragePressure() << " мм рт.ст.\n";
    }

    
    double calculateAverageTemperature() const {
        if (weekWeather.empty()) return 0;
        double sum = 0;
        for (const auto& w : weekWeather) {
            sum += w.getTemperature();
        }
        return sum / weekWeather.size();
    }

    
    double findMaxTemperature() const {
        if (weekWeather.empty()) return 0;
        double max = weekWeather[0].getTemperature();
        for (const auto& w : weekWeather) {
            if (w.getTemperature() > max) {
                max = w.getTemperature();
            }
        }
        return max;
    }

    
    double findMinTemperature() const {
        if (weekWeather.empty()) return 0;
        double min = weekWeather[0].getTemperature();
        for (const auto& w : weekWeather) {
            if (w.getTemperature() < min) {
                min = w.getTemperature();
            }
        }
        return min;
    }

    
    double calculateAverageHumidity() const {
        if (weekWeather.empty()) return 0;
        double sum = 0;
        for (const auto& w : weekWeather) {
            sum += w.getHumidity();
        }
        return sum / weekWeather.size();
    }

    
    double calculateAveragePressure() const {
        if (weekWeather.empty()) return 0;
        double sum = 0;
        for (const auto& w : weekWeather) {
            sum += w.getPressure();
        }
        return sum / weekWeather.size();
    }

    
    void updateData(int index, const Weather& newWeather) {
        if (index >= 0 && index < weekWeather.size()) {
            weekWeather[index] = newWeather;
        }
    }

    
    size_t getSize() const {
        return weekWeather.size();
    }
};

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    setlocale(LC_ALL, "ru_RU.UTF-8");
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    WeeklyWeather weeklyWeather("weekly_weather.json");

    int choice;

    do {
        std::cout << "\n╔════════════════════════════════╗\n";
        std::cout << "║     МЕНЮ ПРОГНОЗА ПОГОДЫ       ║\n";
        std::cout << "╠════════════════════════════════╣\n";
        std::cout << "║ 1. Показать прогноз на неделю ║\n";
        std::cout << "║ 2. Сохранить в JSON файл      ║\n";
        std::cout << "║ 3. Показать статистику        ║\n";
        std::cout << "║ 4. Вывести количество записей ║\n";
        std::cout << "║ 0. Выход                       ║\n";
        std::cout << "╚════════════════════════════════╝\n";
        std::cout << "Выберите действие: ";

        std::cin >> choice;

        switch (choice) {
        case 1:
            weeklyWeather.displayWeeklyData();
            break;

        case 2:
            if (weeklyWeather.saveToFile()) {
                std::cout << "✓ Файл успешно сохранен!\n";
            }
            break;

        case 3:
            std::cout << "\n📊 СТАТИСТИКА ЗА НЕДЕЛЮ:\n";
            std::cout << "──────────────────────────\n";
            std::cout << "Средняя температура: " << std::fixed << std::setprecision(1)
                << weeklyWeather.calculateAverageTemperature() << "°C\n";
            std::cout << "Максимальная температура: " << weeklyWeather.findMaxTemperature() << "°C\n";
            std::cout << "Минимальная температура: " << weeklyWeather.findMinTemperature() << "°C\n";
            std::cout << "Средняя влажность: " << weeklyWeather.calculateAverageHumidity() << "%\n";
            std::cout << "Среднее давление: " << weeklyWeather.calculateAveragePressure() << " мм рт.ст.\n";
            break;

        case 4:
            std::cout << "\nКоличество записей: " << weeklyWeather.getSize() << "\n";
            break;

        case 0:
            std::cout << "\n👋 Программа завершена. До свидания!\n";
            break;

        default:
            std::cout << "✗ Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);

    return 0;
}
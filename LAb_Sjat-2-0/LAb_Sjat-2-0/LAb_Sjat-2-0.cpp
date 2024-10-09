#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <iomanip>

void encode(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: файл " << inputFileName << " не найден." << std::endl;
        return;
    }

    std::string text((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    double l = 0.0, h = 1.0;
    std::vector<int> freq(256, 1); // Частоты символов (начиная с 1)
    int totalSymbols = text.size();

    // Подсчет частот символов
    for (char c : text) {
        freq[static_cast<unsigned char>(c)]++;
    }

    // Кодирование
    for (char c : text) {
        double rangeWidth = h - l;
        h = l + rangeWidth * std::accumulate(freq.begin(), freq.begin() + static_cast<unsigned char>(c) + 1, 0) / totalSymbols;
        l = l + rangeWidth * std::accumulate(freq.begin(), freq.begin() + static_cast<unsigned char>(c), 0) / totalSymbols;

        // Обработка переполнения
        while (true) {
            if (h < 0.5) {
                l *= 2;
                h *= 2;
            }
            else if (l >= 0.5) {
                l = 2 * (l - 0.5);
                h = 2 * (h - 0.5);
            }
            else {
                break;
            }
        }
    }

    // Запись закодированного сообщения в файл
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка записи в файл " << outputFileName << "." << std::endl;
        return;
    }

    outputFile << std::fixed << std::setprecision(10) << l; // Записываем закодированное значение с точностью
    outputFile.close();
}

void decode(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: файл " << inputFileName << " не найден." << std::endl;
        return;
    }

    double code;
    inputFile >> code;
    inputFile.close();

    double l = 0.0, h = 1.0;
    std::vector<int> freq(256, 1); // Частоты символов (начиная с 1)
    std::string decodedText;

    // Декодирование
    while (decodedText.size() < 100) { // Ограничим количество декодируемых символов для примера
        double rangeWidth = h - l;
        int totalSymbols = std::accumulate(freq.begin(), freq.end(), 0);

        // Находим текущий символ
        int cumFreq = 0;
        for (int i = 0; i < 256; ++i) {
            cumFreq += freq[i];
            if (l + rangeWidth * cumFreq / totalSymbols > code) {
                decodedText += static_cast<char>(i);
                freq[i]++; // Увеличиваем частоту символа
                break;
            }
        }

       
       

        // Обработка переполнения аналогично функции encode
        while (true) {
            if (h < 0.5) {
                l *= 2;
                h *= 2;
            }
            else if (l >= 0.5) {
                l = 2 * (l - 0.5);
                h = 2 * (h - 0.5);
            }
            else {
                break;
            }
        }
    }

    // Запись восстановленного текста в файл
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка записи в файл " << outputFileName << "." << std::endl;
        return;
    }

    outputFile << decodedText;
    outputFile.close();
}

int main() {
    std::string inputFileName, outputFileName;

    // Ввод названий файлов от пользователя для кодирования
    std::cout << "Введите имя файла для кодирования: ";
    std::cin >> inputFileName;

    std::cout << "Введите имя файла для сохранения закодированного текста: ";
    std::cin >> outputFileName;
    encode(inputFileName, outputFileName);

    // Ввод названий файлов для декодирования
    std::cout << "Введите имя файла для декодирования: ";
    std::cin >> inputFileName;

    std::cout << "Введите имя файла для сохранения декодированного текста: ";
    std::cin >> outputFileName;

    decode(inputFileName, outputFileName);

    return 0;
}
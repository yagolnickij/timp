#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <vector>
#include <array>
#include "errors.h"

/** 
* @file ioman.h
* @brief Определение класса для управления вводом и выводом данных.
* @details Этот файл содержит определения методов для чтения и записи данных, а также для чтения конфигурационных файлов.
* @date 23.11.2024
* @version 1.0
* @authorsa Ягольницкий Р. С.
*/

/** 
* @brief Класс для управления вводом и выводом данных.
*/
class IOMan {
public:
    /**
    * @brief Конструктор класса IOMan.
    * @param path_to_conf Путь к файлу конфигурации.
    * @param path_to_in Путь к входному файлу.
    * @param path_to_out Путь к выходному файлу.
    */
    IOMan(
        const std::string& path_to_conf, 
        const std::string& path_to_in, 
        const std::string& path_to_out
    );

    /**
    * @brief Метод для чтения конфигурационных данных.
    * @return Массив строк с конфигурационными данными.
    * @throw FileNotFoundError Если не удалось открыть файл конфигурации.
    * @throw InvalidDataFormatError Если отсутствуют логин или пароль.
    */
    std::array<std::string, 2> conf();

    /**
    * @brief Метод для чтения данных из файла.
    * @return Двумерный вектор с данными.
    * @throw std::runtime_error Если не удалось открыть входной файл.
    */
    std::vector<std::vector<int16_t>> read();

    /**
    * @brief Метод для записи данных в файл.
    * @param data Вектор данных для записи.
    * @throw FileNotFoundError Если не удалось открыть выходной файл.
    */
    void write(const std::vector<int16_t>& data);

private:
    std::string path_to_conf; ///< Путь к файлу конфигурации.
    std::string path_to_in; ///< Путь к входному файлу.
    std::string path_to_out; ///< Путь к выходному файлу.
};

#endif // IO_MANAGER_H

#ifndef UI_H
#define UI_H

#include "ioman.h"
#include "netman.h"
#include "errors.h"
#include <string>
#include <vector>

/** 
* @file ui.h
* @brief Определение класса для пользовательского интерфейса.
* @details Этот файл содержит определения методов для обработки аргументов командной строки, показа справки, запуска программы, и управления вводом/выводом и сетевым взаимодействием.
* @date 23.11.2024
* @version 1.0
* @authorsa Ягольницкий Р. С.
*/

/** 
* @brief Класс для управления пользовательским интерфейсом.
*/
class UserInterface
{
public:
    /**
    * @brief Конструктор класса UserInterface.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeError Если отсутствуют обязательные параметры или их значения.
    */
    UserInterface(int argc, char *argv[]);
    
    /**
    * @brief Деструктор класса UserInterface.
    */
    ~UserInterface();

    /**
    * @brief Метод для получения адреса сервера.
    * @return Адрес сервера.
    */
    std::string &getAddress();

    /**
    * @brief Метод для получения порта сервера.
    * @return Порт сервера.
    */
    uint16_t &getPort();

    /**
    * @brief Метод для получения пути к входному файлу.
    * @return Путь к входному файлу.
    */
    std::string &getInputFilePath();

    /**
    * @brief Метод для получения пути к выходному файлу.
    * @return Путь к выходному файлу.
    */
    std::string &getOutputFilePath();

    /**
    * @brief Метод для получения пути к конфигурационному файлу.
    * @return Путь к конфигурационному файлу.
    */
    std::string &getConfigFilePath();
    
    /**
    * @brief Метод для запуска программы.
    */
    void run();

private:
    std::string address; ///< Адрес сервера.
    uint16_t port; ///< Порт сервера.
    std::string input_path; ///< Путь к входному файлу.
    std::string output_path; ///< Путь к выходному файлу.
    std::string config_path; ///< Путь к файлу конфигурации.

    IOMan *io_man; ///< Менеджер ввода-вывода.
    NetMan *net_man; ///< Менеджер сетевого взаимодействия.

    bool help_flag; ///< Флаг для отображения справки.

    /**
    * @brief Вспомогательный метод для разбора аргументов командной строки.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeError Если отсутствуют значения для параметров или найден неизвестный параметр.
    */
    void parseArgs(int argc, char *argv[]);

    /**
    * @brief Метод для отображения справки.
    */
    void showHelp();
};

#endif // UI_H

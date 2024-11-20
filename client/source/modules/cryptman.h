#ifndef CRYPT_MANAGER_H
#define CRYPT_MANAGER_H

#include <string>

/** 
* @file crypt_manager.h
* @brief Определение класса для криптографических операций.
* @details Этот файл содержит определения методов для генерации соли и вычисления хеша.
* @date 23.11.2024
* @version 1.0
* @authorsa Ягольницкий Р. С.
*/

/** 
* @brief Класс для управления криптографическими операциями.
*/
class CryptMan
{
public:
    /**
    * @brief Статический метод для генерации соли.
    * @return Соль в виде строки.
    */
    static std::string get_salt();

    /**
    * @brief Статический метод для вычисления хеша.
    * @param salt Соль, используемая для хеширования.
    * @param data Данные для хеширования.
    * @return Хеш в виде строки.
    */
    static std::string get_hash(const std::string &salt, const std::string &data);
};

#endif // CRYPT_MANAGER_H

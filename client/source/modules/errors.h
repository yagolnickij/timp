#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <string>

/** 
* @file errors.h
* @brief Определение классов для обработки клиентских ошибок.
* @details Этот файл содержит определения классов исключений для обработки различных клиентских ошибок.
* @date 23.11.2024
* @version 1.0
* @authorsa Ягольницкий Р. С.
*/

/** 
* @brief Базовый класс для клиентских ошибок.
*/
class BasicClientError : public std::exception
{
public:
    /**
    * @brief Конструктор класса BasicClientError.
    * @param name Имя ошибки.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    BasicClientError(const std::string &name, const std::string &message, const std::string &func);

    /**
    * @brief Метод для получения сообщения об ошибке.
    * @return Сообщение об ошибке.
    */
    const char *what() const noexcept override;

protected:
    std::string name; ///< Имя ошибки.
    std::string func; ///< Имя функции, в которой возникла ошибка.
    mutable std::string message; ///< Сообщение об ошибке.
};

/** 
* @brief Класс для обработки ошибок "файл не найден".
*/
class FileNotFoundError : public BasicClientError
{
public:
    /**
    * @brief Конструктор класса FileNotFoundError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    FileNotFoundError(const std::string &message, const std::string &func);
};

/** 
* @brief Класс для обработки ошибок декодирования аргументов.
*/
class ArgsDecodeError : public BasicClientError
{
public:
    /**
    * @brief Конструктор класса ArgsDecodeError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    ArgsDecodeError(const std::string &message, const std::string &func);
};

/** 
* @brief Класс для обработки ошибок некорректного формата данных.
*/
class InvalidDataFormatError : public BasicClientError
{
public:
    /**
    * @brief Конструктор класса InvalidDataFormatError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    InvalidDataFormatError(const std::string &message, const std::string &func);
};

/** 
* @brief Класс для обработки ошибок аутентификации.
*/
class AuthError : public BasicClientError
{
public:
    /**
    * @brief Конструктор класса AuthError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    AuthError(const std::string &message, const std::string &func);
};

/** 
* @brief Класс для обработки сетевых ошибок.
*/
class NetworkError : public BasicClientError
{
public:
    /**
    * @brief Конструктор класса NetworkError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    NetworkError(const std::string &message, const std::string &func);
};

#endif // ERRORS_H

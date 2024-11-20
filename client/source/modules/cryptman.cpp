#include "cryptman.h"
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cryptopp/osrng.h>
#include <sstream>
#include <iomanip>

// Реализация статического метода для генерации соли
std::string CryptMan::get_salt()
{
    // Размер соли в байтах
    // 64 бита = 8 байт
    const size_t SALT_SIZE = 8;
    CryptoPP::byte salt[SALT_SIZE]; // массив для соли
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(salt, SALT_SIZE); // генерация соли
    std::string salt_hex;                // строка для соли
    CryptoPP::ArraySource(
        salt,
        SALT_SIZE,
        true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(salt_hex),
            true // Заглавные буквы
            ));

    // Дополнение соли нулями слева до 16 символов
    while (salt_hex.size() < 16)
    {
        salt_hex = "0" + salt_hex;
    }

    return salt_hex;
}

// Реализация статического метода для вычисления хеша
std::string CryptMan::get_hash(const std::string &salt, const std::string &data)
{
    CryptoPP::MD5 hash_func; // создаем объект хеш-функции
    std::string hash_hex;

    // формирование хэша и преобразование в шестнадцатеричную строку
    CryptoPP::StringSource(
        salt + data,
        true,
        new CryptoPP::HashFilter(
            hash_func,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash_hex),
                true // Заглавные буквы
                )));

    return hash_hex;
}

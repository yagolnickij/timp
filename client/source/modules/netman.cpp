#include "netman.h"
#include <cstring>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cryptman.h"
#include "errors.h"
#include <iostream>

// Конструктор
NetMan::NetMan(const std::string &address, uint16_t port)
    : address(address), port(port), socket(-1) {}

std::string &NetMan::getAddress()
{
    return this->address;
};
uint16_t &NetMan::getPort()
{
    return this->port;
};
// Метод для установки соединения
void NetMan::conn()
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket < 0)
    {
        throw NetworkError("Failed to create socket", "NetMan.conn()");
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->address.c_str(), &server_addr.sin_addr) <= 0)
        throw NetworkError("Invalid address/ Address not supported", "NetMan.conn()");

    if (connect(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw NetworkError("Connection failed", "NetMan.conn()");
}

// Метод для аутентификации
void NetMan::auth(const std::string &login, const std::string &password)
{
    std::string salt = CryptMan::get_salt();
    std::string hash = CryptMan::get_hash(salt, password);

    std::string auth_message = login + salt + hash;
    if (send(this->socket, auth_message.c_str(), auth_message.size(), 0) < 0)
        throw AuthError("Failed to send auth message", "NetMan.auth()");

    char response[1024];
    int response_length = recv(this->socket, response, sizeof(response) - 1, 0);
    if (response_length < 0)
    {
        throw AuthError("Failed to receive auth response", "NetMan.auth()");
    }

    response[response_length] = '\0';
    if (std::string(response) != "OK")
    {
        throw AuthError("Authentication failed", "NetMan.auth()");
    }
}

// Метод для передачи данных и получения результата
std::vector<int16_t> NetMan::calc(const std::vector<std::vector<int16_t>> &data)
{
    // Передача количества векторов
    uint32_t num_vectors = data.size();
    if (send(this->socket, &num_vectors, sizeof(num_vectors), 0) < 0)
    {
        throw NetworkError("Failed to send number of vectors", "NetMan.calc()");
    }

    // Передача каждого вектора
    for (const auto &vec : data)
    {
        uint32_t vec_size = vec.size();
        if (send(this->socket, &vec_size, sizeof(vec_size), 0) < 0)
        {
            throw NetworkError("Failed to send vector size", "NetMan.calc()");
        }
        if (send(this->socket, vec.data(), vec_size * sizeof(int16_t), 0) < 0)
        {
            throw NetworkError("Failed to send vector data", "NetMan.calc()");
        }
    }

    // Получение результатов
    std::vector<int16_t> results(num_vectors);
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        if (recv(this->socket, &results[i], sizeof(int16_t), 0) < 0)
        {
            throw NetworkError("Failed to receive result", "NetMan.calc()");
        }
    }

    // Логирование результата
    std::cout << "Log: \"NetMan.calc()\"\n";
    std::cout << "Results: {";
    for (const auto &val : results)
    {
        std::cout << val << ", ";
    }
    if (!results.empty())
    {
        std::cout << "\b\b"; // Удалить последнюю запятую и пробел
    }
    std::cout << "}\n";

    return results;
}

// Метод для закрытия соединения
void NetMan::close()
{
    if (this->socket >= 0)
    {
        ::close(this->socket);
        this->socket = -1;
    }
}

#include "ioman.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Конструктор
IOMan::IOMan(
    const std::string &path_to_conf,
    const std::string &path_to_in,
    const std::string &path_to_out)
    : path_to_conf(path_to_conf),
      path_to_in(path_to_in),
      path_to_out(path_to_out) {}

// Метод для чтения конфигурационных данных
std::array<std::string, 2> IOMan::conf()
{
    std::ifstream conf_file(this->path_to_conf);
    if (!conf_file.is_open())
    {
        throw FileNotFoundError(
            "Failed to open configuration file \"" +
                this->path_to_conf + "\"",
            "IOMan.conf()");
    }

    std::array<std::string, 2> credentials;
    std::string line;
    std::getline(conf_file, line);
    conf_file.close();

    std::istringstream iss(line);
    std::getline(iss, credentials[0], ':');
    std::getline(iss, credentials[1]);

    // Проверка на наличие обоих значений
    if (credentials[0].empty() || credentials[1].empty())
    {
        throw InvalidDataFormatError(
            "Missing login or password",
            "IOMan.conf()");
    }

    // Логирование считанных логина и пароля
    std::cout << "Log: \"IOMan.conf()\"\n";
    std::cout << "UserData: " << credentials[0]
              << " " << credentials[1] << "\n";

    return credentials;
}

// Метод для чтения числовых данных с логированием из текстового файла
std::vector<std::vector<int16_t>> IOMan::read()
{
    std::ifstream input_file(this->path_to_in);
    if (!input_file.is_open())
    {
        throw std::runtime_error("Failed to open input file for reading.");
    }

    // Чтение количества векторов
    uint32_t num_vectors;
    input_file >> num_vectors;

    std::vector<std::vector<int16_t>> data(num_vectors);

    // Чтение каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение размера вектора
        uint32_t vector_size;
        input_file >> vector_size;

        // Чтение значений вектора
        std::vector<int16_t> vec(vector_size);
        for (uint32_t j = 0; j < vector_size; ++j)
        {
            input_file >> vec[j]; // Чтение в десятичном формате
        }

        data[i] = vec;
    }

    input_file.close();

    // Логирование всех прочитанных векторов
    std::cout << "Log: IOMan.read()\n";
    std::cout << "Vectors: {";
    for (const auto &vec : data)
    {
        std::cout << "{";
        for (const auto &val : vec)
            std::cout << val << ", ";
        if (!vec.empty())
            std::cout << "\b\b"; // Удалить последнюю запятую и пробел
        std::cout << "}, ";
    }
    if (!data.empty())
        std::cout << "\b\b"; // Удалить последнюю запятую и пробел
    std::cout << "}\n";

    return data;
}

// Метод для записи числовых данных
void IOMan::write(const std::vector<int16_t> &data)
{
    std::ofstream output_file(this->path_to_out, std::ios::binary);
    if (!output_file.is_open())
    {
        throw FileNotFoundError(
            "Failed to open output file \"" +
                this->path_to_out + "\"",
            "IOMan.write()");
    }

    // Запись количества результатов
    uint32_t count = data.size();
    output_file.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Запись самих результатов
    for (const auto &num : data)
    {
        output_file.write(reinterpret_cast<const char *>(&num), sizeof(num));
    }

    output_file.close();
}

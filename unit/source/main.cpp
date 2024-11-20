/**
 * @file main_test.cpp
 * @brief Тесты для проверки функциональности различных компонентов.
 * @details Этот файл содержит тесты для проверки генерации соли, вычисления хеша, чтения и записи данных, а также работы сетевого взаимодействия и пользовательского интерфейса.
 * @date 23.11.2024
 * @version 1.0
 * @authorsa Ягольницкий Р. С.
 */

#include <UnitTest++/UnitTest++.h>
#include "../../client/source/modules/cryptman.h"
#include "../../client/source/modules/netman.h"
#include "../../client/source/modules/ioman.h"
#include "../../client/source/modules/ui.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

/**
 * @brief Тест для генерации соли.
 */
TEST(GetSalt)
{
    std::string salt1 = CryptMan::get_salt();
    std::string salt2 = CryptMan::get_salt();

    // Проверяем, что соль не пустая
    CHECK(!salt1.empty());
    CHECK(!salt2.empty());

    // Проверяем, что две разные соли не совпадают
    CHECK(salt1 != salt2);

    // Проверяем, что соль имеет длину 16 символов
    CHECK_EQUAL(16, salt1.size());
    CHECK_EQUAL(16, salt2.size());

    // Выводим результат
    std::cout << "Salt 1: " << salt1 << std::endl;
    std::cout << "Salt 2: " << salt2 << std::endl;
}

/**
 * @brief Тест для вычисления хеша.
 */
TEST(GetHash)
{
    std::string salt = "A1B2C3D4E5F6G7H8";
    std::string data = "P@ssW0rd";
    std::string hash1 = CryptMan::get_hash(salt, data);

    // Проверяем, что хеш не пустой
    CHECK(!hash1.empty());

    // Проверяем, что хеш имеет длину 32 символа (MD5 хеш)
    CHECK_EQUAL(32, hash1.size());

    // Проверяем, что при одинаковых входных данных хеши совпадают
    std::string hash2 = CryptMan::get_hash(salt, data);
    CHECK_EQUAL(hash1, hash2);

    // Проверяем, что при разных данных хеши не совпадают
    std::string different_data = "DifferentPassword";
    std::string hash3 = CryptMan::get_hash(salt, different_data);
    CHECK(hash1 != hash3);

    // Выводим результат
    std::cout << "Hash 1: " << hash1 << std::endl;
    std::cout << "Hash 2: " << hash2 << std::endl;
    std::cout << "Hash 3: " << hash3 << std::endl;
}

/**
 * @brief Тест для чтения конфигурационных данных.
 */
TEST(IOManConf)
{
    IOMan ioMan("./config/vclient.conf", "./input.txt", "./output.bin");
    std::array<std::string, 2> credentials = ioMan.conf();

    // Проверяем, что конфигурация не пустая
    CHECK_EQUAL("user", credentials[0]);
    CHECK_EQUAL("P@ssW0rd", credentials[1]);
}

/**
 * @brief Тест для чтения числовых данных.
 */
TEST(IOManRead)
{
    IOMan ioMan("./config/vclient.conf", "./input.txt", "./output.bin");
    std::vector<std::vector<int16_t>> data = ioMan.read();

    // Проверка, что данные были прочитаны
    CHECK(!data.empty());
}

/**
 * @brief Тест для записи числовых данных.
 */
TEST(IOManWrite)
{
    IOMan ioMan("./config/vclient.conf", "./input.txt", "./output.bin");
    std::vector<int16_t> data = {6, 7, 8};
    ioMan.write(data);

    // Здесь можно добавить дополнительные проверки, чтобы убедиться, что данные были успешно записаны
}

/**
 * @brief Тест для ошибки открытия конфигурационного файла.
 */
TEST(IOManConfFileNotFound)
{
    IOMan ioMan("./non_exists_path.conf", "./input.txt", "./output.bin");
    CHECK_THROW(ioMan.conf(), FileNotFoundError);
}

/**
 * @brief Тест для ошибки отсутствия данных в конфигурационном файле.
 */
TEST(IOManConfMissingData)
{
    const std::string conf_path = "./conf.cfg";
    std::ofstream conf_file(conf_path);
    conf_file << "username:";
    conf_file.close();

    IOMan ioMan(conf_path, "./input.txt", "./output.bin");
    CHECK_THROW(ioMan.conf(), InvalidDataFormatError);

    std::remove(conf_path.c_str());
}

/**
 * @brief Тест для ошибки открытия входного файла.
 */
TEST(IOManReadFileNotFound)
{
    IOMan ioMan("./config/vclient.conf", "./non_exists_path.bin", "./output.bin");
    CHECK_THROW(ioMan.read(), std::runtime_error);
}

/**
 * @brief Тест для ошибки открытия выходного файла.
 */
TEST(IOManWriteFileNotFound)
{
    IOMan ioMan("./config/vclient.conf", "./input.txt", "./non_exists_path/non_exists_file.bin");
    CHECK_THROW(ioMan.write({1, 2, 3, 4, 5}), FileNotFoundError);
}

/**
 * @brief Тест для установки соединения.
 */
TEST(NetManConnect)
{
    NetMan netManager("127.0.0.1", 33333);
    netManager.conn();

    // Проверка параметров после соединения
    CHECK_EQUAL(std::string("127.0.0.1"), netManager.getAddress());
    CHECK_EQUAL((uint16_t)33333, netManager.getPort());

    netManager.close();
}

/**
 * @brief Тест для аутентификации.
 */
TEST(NetManAuth)
{
    NetMan netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.auth("user", "P@ssW0rd");

    netManager.close();
}

/**
 * @brief Тест для передачи данных и получения результата.
 */
TEST(NetManCalc)
{
    NetMan netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.auth("user", "P@ssW0rd");

    std::vector<std::vector<int16_t>> data = {{1, 2, 3}, {4, 5, 6}};
    std::vector<int16_t> results = netManager.calc(data);

    // Проверяем, что результаты не пустые
    CHECK(!results.empty());

    netManager.close();
}

/**
 * @brief Тест для ошибки соединения.
 */
TEST(NetManConnError)
{
    NetMan netManager("256.256.256.256", 33333);
    CHECK_THROW(netManager.conn(), NetworkError);
}

/**
 * @brief Тест для ошибки аутентификации.
 */
TEST(NetManAuthError)
{
    NetMan netManager("127.0.0.1", 33333);
    netManager.conn();
    CHECK_THROW(netManager.auth("invalid_user", "invalid_pass"), AuthError);
    netManager.close();
}

/**
 * @brief Тест для закрытия соединения.
 */
TEST(NetManClose)
{
    NetMan netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.close();
}

/**
 * @brief Тест для проверки корректной обработки параметров.
 */
TEST(UserInterfaceParseArgsCorrect)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "33333", "-i", "input.bin", "-o", "output.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    UserInterface ui(argc, const_cast<char **>(argv));

    // Проверяем корректность распарсенных параметров
    CHECK_EQUAL(std::string("192.168.0.1"), ui.getAddress());
    CHECK_EQUAL((uint16_t)33333, ui.getPort());
    CHECK_EQUAL(std::string("input.bin"), ui.getInputFilePath());
    CHECK_EQUAL(std::string("output.bin"), ui.getOutputFilePath());
    CHECK_EQUAL(std::string("config.conf"), ui.getConfigFilePath());
}

/**
 * @brief Тест для проверки отсутствия обязательного параметра input.
 */
TEST(UserInterfaceParseArgsMissingInput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "33333", "-o", "output.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Тест для проверки отсутствия обязательного параметра output.
 */
TEST(UserInterfaceParseArgsMissingOutput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "33333", "-i", "input.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Тест для проверки отсутствия значения для параметра address.
 */
TEST(UserInterfaceParseArgsMissingAddressValue)
{
    const char *argv[] = {"vclient", "-a"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Тест для проверки отсутствия значения для параметра port.
 */
TEST(UserInterfaceParseArgsMissingPortValue)
{
    const char *argv[] = {"vclient", "-p"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Тест для проверки отсутствия значения для параметра config.
 */
TEST(UserInterfaceParseArgsMissingConfigValue)
{
    const char *argv[] = {"vclient", "-c"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Тест для проверки неизвестного параметра.
 */
TEST(UserInterfaceParseArgsUnknownParam)
{
    const char *argv[] = {"vclient", "--unknown"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    /**
     * @brief Сообщает о начале выполнения теста.
     * @param test Детали теста.
     */
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    /**
     * @brief Сообщает об окончании выполнения теста.
     * @param test Детали теста.
     * @param secondsElapsed Время выполнения теста в секундах.
     */
    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed("
                  << secondsElapsed << " seconds)\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает о сбое теста.
     * @param test Детали теста.
     * @param failure Описание сбоя.
     */
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: "
                  << " (" << failure << ")\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает об общей сводке выполнения тестов.
     * @param totalTestCount Общее количество тестов.
     * @param failedTestCount Количество неудачных тестов.
     * @param failureCount Общее количество сбоев.
     * @param secondsElapsed Общее время выполнения тестов в секундах.
     */
    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: "
                  << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};

/**
 * @brief Главная функция тестирования.
 * @details Инициализирует объект MyTestReporter и запускает тесты с использованием UnitTest++.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}

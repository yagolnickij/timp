#include "errors.h"

// Реализация конструктора BasicClientError
BasicClientError::BasicClientError(
    const std::string &name,
    const std::string &message,
    const std::string &func)
    : name(name), message(message), func(func) {}

const char *BasicClientError::what() const noexcept
{
    message = name + " in " + func + "\nMessage: " + message + ".";
    return message.c_str();
}

FileNotFoundError::FileNotFoundError(const std::string &message, const std::string &func)
    : BasicClientError("FileNotFoundError", message, func) {}

ArgsDecodeError::ArgsDecodeError(const std::string &message, const std::string &func)
    : BasicClientError("ArgsDecodeError", message, func) {}

InvalidDataFormatError::InvalidDataFormatError(const std::string &message, const std::string &func)
    : BasicClientError("InvalidDataFormatError", message, func) {}

AuthError::AuthError(const std::string &message, const std::string &func)
    : BasicClientError("AuthError", message, func) {}

NetworkError::NetworkError(const std::string &message, const std::string &func)
    : BasicClientError("NetworkError", message, func) {}

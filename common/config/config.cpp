#include <stdexcept>


#include "config.h"

Config::Config() {

    auto GetEnv = [](const std::string& name) {
        char* ptr = std::getenv(name.c_str());
        if (ptr == nullptr)
            throw std::runtime_error("error: '" + name + "' does not exported");
        return ptr;
    };

    _host = GetEnv("DB_HOST");
    _port = GetEnv("DB_PORT");
    _login = GetEnv("DB_LOGIN");
    _password = GetEnv("DB_PASSWORD");
    _database = GetEnv("DB_DATABASE");
}

Config &Config::get()
{
    static Config _instance;
    return _instance;
}

const std::string &Config::get_port() const
{
    return _port;
}

const std::string &Config::get_host() const
{
    return _host;
}

const std::string &Config::get_login() const
{
    return _login;
}

const std::string &Config::get_password() const
{
    return _password;
}
const std::string &Config::get_database() const
{
    return _database;
}

std::string &Config::port()
{
    return _port;
}

std::string &Config::host()
{
    return _host;
}

std::string &Config::login()
{
    return _login;
}

std::string &Config::password()
{
    return _password;
}

std::string &Config::database()
{
    return _database;
}
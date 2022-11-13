//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "Utility.h"
#include "Triple.h"


class Logger
{
public:

    template<typename T>
    void log(T var);

    template<typename T>
    void log(std::vector<T> var);

    template<typename T>
    void log(std::list<T> var);

    template<typename T1, typename T2>
    void log(std::map<T1, T2> var);

    template<typename T1, typename T2>
    void log(std::unordered_map<T1, T2> var);

    template<typename T1, typename T2>
    void log(std::vector<std::unordered_map<T1, T2>> var);

    template<typename T1, typename T2>
    void log(std::pair<T1, T2> var);

    void log(std::vector<Triple> &var);
};

void Logger::log(std::vector<Triple> &var)
{
    for (auto &item: var)
        std::cout << item.to_string() << std::endl;

}

template<typename T1, typename T2>
void Logger::log(std::vector<std::unordered_map<T1, T2>> var)
{
    for (const auto &item: var) {
        for (const auto &it: item)
            std::cout << it.first << ":" << it.second << "\t";
        std::cout << std::endl;
    }
}

template<typename T>
void Logger::log(T var)
{
    std::cout << var << std::endl;
}

template<typename T>
void Logger::log(std::vector<T> var)
{
    for (const auto &item: var)
        log(item);
}

template<typename T>
void Logger::log(std::list<T> var)
{
    for (const auto &item: var)
        log(item);
}

template<typename T1, typename T2>
void Logger::log(std::map<T1, T2> var)
{
    for (const auto &item: var)
        std::cout << item.first << ":" << item.second << std::endl;
}

template<typename T1, typename T2>
void Logger::log(std::pair<T1, T2> var)
{
    std::cout << var.first << ":" << var.second << std::endl;
}

template<typename T1, typename T2>
void Logger::log(std::unordered_map<T1, T2> var)
{
    for (const auto &it: var)
        std::cout << it.first << ":" << it.second << "\n";
}

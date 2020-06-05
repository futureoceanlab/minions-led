#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <iostream>
#include <fstream> // ofstream
#include <iostream>

class Logger
{
public:
    Logger();
    Logger(uint8_t maxCount);

    void log(long long t_nsec, std::string t_rtc, float pressure, float temperature);
    void open(std::string path);
    void close();

private:
    uint8_t logCount;
    uint8_t logFlushCount;
    std::string logPath;
    std::ofstream logF;
};

#endif
#include "logger.h"

Logger::Logger()
{
    logCount = 0;
    logFlushCount = 10;
}

Logger::Logger(uint8_t maxCount)
{
    logCount = 0;
    logFlushCount = maxCount;
}

void Logger::open(std::string path)
{
    logPath = path;
    logF.open(logPath);
    logF << std::fixed;
    logF.precision(2);
    logF << "Timestamp(ns),RTC,Pressure(mbar),Temperature(C)\n";
}

void Logger::log(long long t_nsec, std::string t_rtc, float pressure, float temperature)
{
    logF << t_nsec << ",";
    logF << t_rtc << ",";
    logF << pressure << ",";
    logF << temperature << "\n";
    logCount++;
    if (logCount > logFlushCount)
    {
        logF.flush();
        logCount = 0;
    }
}

void Logger::close()
{
    logF.close();
}

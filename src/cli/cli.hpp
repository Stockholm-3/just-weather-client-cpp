#pragma once

#include <string>

class WeatherClient;

class CLI {
public:
    explicit CLI(WeatherClient& client);

    void runInteractive();
    int runCommandLine(int argc, char* argv[]);
    void printUsage(const std::string& program) const;

    static void printUsageStatic(const std::string& program);

private:
    WeatherClient& client_;
};

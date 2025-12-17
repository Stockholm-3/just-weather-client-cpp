#pragma once

#include <string>

namespace weather {
    class WeatherClient;
}

class CLI {
public:
    explicit CLI(weather::WeatherClient& client);

    void runInteractive();
    int runCommandLine(int argc, char* argv[]);
    void printUsage(const std::string& program) const;

    static void printUsageStatic(const std::string& program);

private:
    weather::WeatherClient& client_;
};

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace weather {
    class WeatherClient;
}

class Command;

class CommandParser {
public:
    static std::unique_ptr<Command>
    parse(weather::WeatherClient& client,
          const std::vector<std::string>& tokens);
};

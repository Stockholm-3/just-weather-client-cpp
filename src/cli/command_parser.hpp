#pragma once

#include <memory>
#include <string>
#include <vector>

class WeatherClient;
class Command;

class CommandParser {
public:
    static std::unique_ptr<Command>
    parse(WeatherClient& client,
          const std::vector<std::string>& tokens);
};

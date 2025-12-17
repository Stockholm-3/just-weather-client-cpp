#pragma once

#include "../command.hpp"
#include <string>

namespace weather {
    class WeatherClient;
}

class CitiesCommand final : public Command {
public:
    CitiesCommand(weather::WeatherClient& client, const std::string& query);
    void execute() override;

private:
    weather::WeatherClient& client_;
    std::string query_;
};

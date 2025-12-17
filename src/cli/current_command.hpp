#pragma once

#include "command.hpp"

namespace weather {
    class WeatherClient;
}

class CurrentCommand final : public Command {
public:
    CurrentCommand(weather::WeatherClient& client, double lat, double lon);
    void execute() override;

private:
    weather::WeatherClient& client_;
    double lat_;
    double lon_;
};

#pragma once

#include "command.hpp"

class WeatherClient;

class CurrentCommand final : public Command {
public:
    CurrentCommand(WeatherClient& client, double lat, double lon);
    void execute() override;

private:
    WeatherClient& client_;
    double lat_;
    double lon_;
};

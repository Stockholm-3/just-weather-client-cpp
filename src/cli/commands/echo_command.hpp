#pragma once

#include "../command.hpp"

namespace weather {
    class WeatherClient;
}

class EchoCommand final : public Command {
public:
    explicit EchoCommand(weather::WeatherClient& client);
    void execute() override;

private:
    weather::WeatherClient& client_;
};

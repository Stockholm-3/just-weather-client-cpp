#pragma once

#include "../command.hpp"

namespace weather {
    class WeatherClient;
}

class HomepageCommand final : public Command {
public:
    explicit HomepageCommand(weather::WeatherClient& client);
    void execute() override;

private:
    weather::WeatherClient& client_;
};

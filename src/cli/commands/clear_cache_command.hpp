#pragma once

#include "../command.hpp"

namespace weather {
    class WeatherClient;
}

class ClearCacheCommand final : public Command {
public:
    explicit ClearCacheCommand(weather::WeatherClient& client);
    void execute() override;

private:
    weather::WeatherClient& client_;
};

#pragma once

#include "../command.hpp"
#include <string>
#include <optional>

namespace weather {
    class WeatherClient;
}

class WeatherCommand final : public Command {
public:
    WeatherCommand(weather::WeatherClient& client,
                   const std::string& city,
                   const std::optional<std::string>& country = std::nullopt,
                   const std::optional<std::string>& region = std::nullopt);
    void execute() override;

private:
    weather::WeatherClient& client_;
    std::string city_;
    std::optional<std::string> country_;
    std::optional<std::string> region_;
};

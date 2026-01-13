/**
 * @file weather_command.hpp
 * @brief Defines the WeatherCommand class for retrieving weather information
 */

#pragma once

#include "../command.hpp"

#include <optional>
#include <string>

namespace weather {
class WeatherClient;
}

/**
 * @class WeatherCommand
 * @brief Command for retrieving weather information for a specific location
 *
 * This command fetches and displays weather information for a given city,
 * with optional country and region specifications for more precise location
 * matching.
 */
class WeatherCommand final : public Command {
  public:
    /**
     * @brief Constructs a WeatherCommand object
     * @param client Reference to the WeatherClient instance
     * @param city The name of the city to get weather information for
     * @param country Optional country name or code for location disambiguation
     * @param region Optional region name for more precise location matching
     */
    WeatherCommand(weather::WeatherClient& client, const std::string& city,
                   const std::optional<std::string>& country = std::nullopt,
                   const std::optional<std::string>& region  = std::nullopt);

    /**
     * @brief Executes the weather information retrieval command
     *
     * Fetches weather data for the specified location and displays
     * it to the user in a formatted manner.
     */
    void execute() override;

  private:
    weather::WeatherClient&    client_;  ///< Reference to the weather client
    std::string                city_;    ///< Name of the city
    std::optional<std::string> country_; ///< Optional country specification
    std::optional<std::string> region_;  ///< Optional region specification
};

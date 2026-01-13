/**
 * @file current_command.hpp
 * @brief Defines the CurrentCommand class for retrieving current weather by
 * coordinates
 */

#pragma once

#include "command.hpp"

namespace weather {
class WeatherClient;
}

/**
 * @class CurrentCommand
 * @brief Command for retrieving current weather information by geographic
 * coordinates
 *
 * This command fetches and displays current weather information for a location
 * specified by latitude and longitude coordinates.
 */
class CurrentCommand final : public Command {
  public:
    /**
     * @brief Constructs a CurrentCommand object
     * @param client Reference to the WeatherClient instance
     * @param lat Latitude coordinate of the location
     * @param lon Longitude coordinate of the location
     */
    CurrentCommand(weather::WeatherClient& client, double lat, double lon);

    /**
     * @brief Executes the current weather retrieval command
     *
     * Fetches current weather data for the specified coordinates and
     * displays it to the user.
     */
    void execute() override;

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
    double                  lat_;    ///< Latitude coordinate
    double                  lon_;    ///< Longitude coordinate
};

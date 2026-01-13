/**
 * @file cities_command.hpp
 * @brief Defines the CitiesCommand class for searching cities
 */

#pragma once

#include "../command.hpp"

#include <string>

namespace weather {
class WeatherClient;
}

/**
 * @class CitiesCommand
 * @brief Command for searching cities by query string
 *
 * This command allows users to search for cities using a query string.
 * It communicates with the WeatherClient to perform the search operation.
 */
class CitiesCommand final : public Command {
  public:
    /**
     * @brief Constructs a CitiesCommand object
     * @param client Reference to the WeatherClient instance
     * @param query Search query string for finding cities
     */
    CitiesCommand(weather::WeatherClient& client, const std::string& query);

    /**
     * @brief Executes the cities search command
     *
     * Performs the actual search operation using the provided query
     * and displays the results to the user.
     */
    void execute() override;

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
    std::string             query_;  ///< Search query string
};

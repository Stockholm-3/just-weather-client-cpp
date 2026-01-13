/**
 * @file clear_cache_command.hpp
 * @brief Defines the ClearCacheCommand class for clearing the cache
 */

#pragma once

#include "../command.hpp"

namespace weather {
class WeatherClient;
}

/**
 * @class ClearCacheCommand
 * @brief Command for clearing the application cache
 *
 * This command provides functionality to clear all cached data
 * stored by the weather client application.
 */
class ClearCacheCommand final : public Command {
  public:
    /**
     * @brief Constructs a ClearCacheCommand object
     * @param client Reference to the WeatherClient instance
     */
    explicit ClearCacheCommand(weather::WeatherClient& client);

    /**
     * @brief Executes the cache clearing command
     *
     * Clears all cached data from the weather client's cache storage.
     */
    void execute() override;

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
};

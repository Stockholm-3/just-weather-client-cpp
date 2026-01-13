/**
 * @file homepage_command.hpp
 * @brief Defines the HomepageCommand class for displaying homepage information
 */

#pragma once

#include "../command.hpp"

namespace weather {
class WeatherClient;
}

/**
 * @class HomepageCommand
 * @brief Command for displaying homepage or application information
 *
 * This command provides functionality to display the application's
 * homepage or related information to the user.
 */
class HomepageCommand final : public Command {
  public:
    /**
     * @brief Constructs a HomepageCommand object
     * @param client Reference to the WeatherClient instance
     */
    explicit HomepageCommand(weather::WeatherClient& client);

    /**
     * @brief Executes the homepage command
     *
     * Displays the homepage or related application information.
     */
    void execute() override;

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
};

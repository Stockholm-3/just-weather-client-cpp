/**
 * @file echo_command.hpp
 * @brief Defines the EchoCommand class for echo functionality
 */

#pragma once

#include "../command.hpp"

namespace weather {
class WeatherClient;
}

/**
 * @class EchoCommand
 * @brief Command for echoing or displaying information
 *
 * This command provides echo functionality, typically used for
 * displaying information or testing purposes.
 */
class EchoCommand final : public Command {
  public:
    /**
     * @brief Constructs an EchoCommand object
     * @param client Reference to the WeatherClient instance
     */
    explicit EchoCommand(weather::WeatherClient& client);

    /**
     * @brief Executes the echo command
     *
     * Performs the echo operation using the weather client.
     */
    void execute() override;

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
};

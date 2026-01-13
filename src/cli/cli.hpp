/**
 * @file cli.hpp
 * @brief Defines the CLI class for command-line interface functionality
 */

#pragma once

#include <string>

namespace weather {
class WeatherClient;
}

/**
 * @class CLI
 * @brief Main command-line interface controller
 *
 * This class provides the main interface for interacting with the weather
 * client application. It supports both interactive mode and command-line
 * argument mode.
 */
class CLI {
  public:
    /**
     * @brief Constructs a CLI object
     * @param client Reference to the WeatherClient instance
     */
    explicit CLI(weather::WeatherClient& client);

    /**
     * @brief Runs the CLI in interactive mode
     *
     * Starts an interactive session where the user can enter commands
     * continuously until they choose to exit.
     */
    void runInteractive();

    /**
     * @brief Runs a single command from command-line arguments
     * @param argc Number of command-line arguments
     * @param argv Array of command-line argument strings
     * @return Exit code (0 for success, non-zero for errors)
     */
    int runCommandLine(int argc, char* argv[]);

    /**
     * @brief Prints usage information for the CLI
     * @param program Name of the program executable
     */
    void printUsage(const std::string& program) const;

    /**
     * @brief Static version of usage information printer
     * @param program Name of the program executable
     *
     * This static method can be called without an instance of CLI.
     */
    static void printUsageStatic(const std::string& program);

  private:
    weather::WeatherClient& client_; ///< Reference to the weather client
};

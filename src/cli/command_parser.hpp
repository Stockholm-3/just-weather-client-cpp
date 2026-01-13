/**
 * @file command_parser.hpp
 * @brief Defines the CommandParser class for parsing command strings
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace weather {
class WeatherClient;
}

class Command;

/**
 * @class CommandParser
 * @brief Parser for converting command strings into Command objects
 *
 * This class provides functionality to parse user input (in the form of
 * tokenized strings) and create appropriate Command objects based on
 * the parsed command name and arguments.
 */
class CommandParser {
  public:
    /**
     * @brief Parses tokens and creates a corresponding Command object
     * @param client Reference to the WeatherClient instance
     * @param tokens Vector of tokenized command strings (command name and
     * arguments)
     * @return Unique pointer to the created Command object, or nullptr if
     * parsing fails
     *
     * The first token is expected to be the command name, and subsequent tokens
     * are treated as arguments for that command.
     */
    static std::unique_ptr<Command>
    parse(weather::WeatherClient&         client,
          const std::vector<std::string>& tokens);
};

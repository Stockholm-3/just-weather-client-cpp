#include "command_parser.hpp"
#include "current_command.hpp"

#include <stdexcept>

std::unique_ptr<Command>
CommandParser::parse(WeatherClient& client,
                     const std::vector<std::string>& t) {
    if (t.empty()) {
        throw std::invalid_argument("Empty command");
    }

    if (t[0] == "current") {
        if (t.size() != 3)
            throw std::invalid_argument("Usage: current <lat> <lon>");

        double lat = std::stod(t[1]);
        double lon = std::stod(t[2]);

        return std::make_unique<CurrentCommand>(client, lat, lon);
    }

    throw std::invalid_argument("Unknown command: " + t[0]);
}

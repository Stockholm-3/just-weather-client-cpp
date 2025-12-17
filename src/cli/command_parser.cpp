#include "command_parser.hpp"
#include "current_command.hpp"
#include "commands/weather_command.hpp"
#include "commands/cities_command.hpp"
#include "commands/homepage_command.hpp"
#include "commands/echo_command.hpp"
#include "commands/clear_cache_command.hpp"

#include <stdexcept>

std::unique_ptr<Command>
CommandParser::parse(weather::WeatherClient& client,
                     const std::vector<std::string>& t) {
    if (t.empty()) {
        throw std::invalid_argument("Empty command");
    }

    const std::string& cmd = t[0];

    if (cmd == "current") {
        if (t.size() != 3)
            throw std::invalid_argument("Usage: current <lat> <lon>");

        double lat = std::stod(t[1]);
        double lon = std::stod(t[2]);

        return std::make_unique<CurrentCommand>(client, lat, lon);
    }

    if (cmd == "weather") {
        if (t.size() < 2)
            throw std::invalid_argument("Usage: weather <city> [country] [region]");

        std::string city = t[1];
        std::optional<std::string> country = t.size() > 2 ? std::optional(t[2]) : std::nullopt;
        std::optional<std::string> region = t.size() > 3 ? std::optional(t[3]) : std::nullopt;

        return std::make_unique<WeatherCommand>(client, city, country, region);
    }

    if (cmd == "cities") {
        if (t.size() < 2)
            throw std::invalid_argument("Usage: cities <query>");

        // Join all remaining tokens as query
        std::string query = t[1];
        for (size_t i = 2; i < t.size(); ++i) {
            query += " " + t[i];
        }

        return std::make_unique<CitiesCommand>(client, query);
    }

    if (cmd == "homepage") {
        return std::make_unique<HomepageCommand>(client);
    }

    if (cmd == "echo") {
        return std::make_unique<EchoCommand>(client);
    }

    if (cmd == "clear-cache") {
        return std::make_unique<ClearCacheCommand>(client);
    }

    throw std::invalid_argument("Unknown command: " + cmd);
}

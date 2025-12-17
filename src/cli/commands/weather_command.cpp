#include "weather_command.hpp"
#include "../../api/weather_client.hpp"

#include <iostream>
#include <jansson.h>

WeatherCommand::WeatherCommand(weather::WeatherClient& c,
                               const std::string& city,
                               const std::optional<std::string>& country,
                               const std::optional<std::string>& region)
    : client_(c), city_(city), country_(country), region_(region) {}

void WeatherCommand::execute() {
    auto json = client_.getWeatherByCity(city_, country_, region_);

    char* json_str = json_dumps(json.get(), JSON_INDENT(2) | JSON_PRESERVE_ORDER);
    if (json_str) {
        std::cout << json_str << std::endl;
        free(json_str);
    } else {
        std::cerr << "Failed to serialize JSON" << std::endl;
    }
}

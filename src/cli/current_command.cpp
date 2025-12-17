#include "current_command.hpp"
#include "api/weather_client.hpp"

#include <iostream>
#include <jansson.h>

CurrentCommand::CurrentCommand(weather::WeatherClient& c, double lat, double lon)
    : client_(c), lat_(lat), lon_(lon) {}

void CurrentCommand::execute() {
    auto json = client_.getCurrentWeather(lat_, lon_);

    // Convert JSON to string with pretty printing
    char* json_str = json_dumps(json.get(), JSON_INDENT(2) | JSON_PRESERVE_ORDER);
    if (json_str) {
        std::cout << json_str << std::endl;
        free(json_str);
    } else {
        std::cerr << "Failed to serialize JSON" << std::endl;
    }
}

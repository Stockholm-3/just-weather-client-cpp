#include "homepage_command.hpp"
#include "../../api/weather_client.hpp"

#include <iostream>
#include <jansson.h>

HomepageCommand::HomepageCommand(weather::WeatherClient& c)
    : client_(c) {}

void HomepageCommand::execute() {
    auto json = client_.getHomepage();

    char* json_str = json_dumps(json.get(), JSON_INDENT(2) | JSON_PRESERVE_ORDER);
    if (json_str) {
        std::cout << json_str << std::endl;
        free(json_str);
    } else {
        std::cerr << "Failed to serialize JSON" << std::endl;
    }
}

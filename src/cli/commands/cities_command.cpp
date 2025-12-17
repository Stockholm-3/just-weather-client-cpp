#include "cities_command.hpp"
#include "../../api/weather_client.hpp"

#include <iostream>
#include <jansson.h>

CitiesCommand::CitiesCommand(weather::WeatherClient& c, const std::string& query)
    : client_(c), query_(query) {}

void CitiesCommand::execute() {
    auto json = client_.searchCities(query_);

    char* json_str = json_dumps(json.get(), JSON_INDENT(2) | JSON_PRESERVE_ORDER);
    if (json_str) {
        std::cout << json_str << std::endl;
        free(json_str);
    } else {
        std::cerr << "Failed to serialize JSON" << std::endl;
    }
}

#include "current_command.hpp"
#include "api/weather_client.hpp"

#include <iostream>

CurrentCommand::CurrentCommand(WeatherClient& c, double lat, double lon)
    : client_(c), lat_(lat), lon_(lon) {}

void CurrentCommand::execute() {
    auto json = client_.getCurrent(lat_, lon_);
    std::cout << json.dump(2) << std::endl;
}

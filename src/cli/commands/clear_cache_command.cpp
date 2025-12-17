#include "clear_cache_command.hpp"
#include "../../api/weather_client.hpp"

#include <iostream>

ClearCacheCommand::ClearCacheCommand(weather::WeatherClient& c)
    : client_(c) {}

void ClearCacheCommand::execute() {
    client_.clearCache();
    std::cout << "Cache cleared" << std::endl;
}

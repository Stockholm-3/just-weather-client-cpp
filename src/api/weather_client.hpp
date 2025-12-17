#pragma once

#include <string>

// Mock JSON class for testing
class JSON {
public:
    std::string dump(int indent = 0) const {
        return "{\n  \"temperature\": 25.5,\n  \"condition\": \"sunny\"\n}";
    }
};

// Mock WeatherClient for testing CLI
class WeatherClient {
public:
    WeatherClient(const std::string& host, int port)
        : host_(host), port_(port) {}

    JSON getCurrent(double lat, double lon) {
        // Mock implementation - just returns fake data
        return JSON{};
    }

private:
    std::string host_;
    int port_;
};

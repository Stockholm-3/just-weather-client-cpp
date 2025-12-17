#pragma once

#include <jansson.h>
#include <memory>
#include <string>
#include <optional>
#include <stdexcept>

namespace weather {

/**
 * Exception class for weather client errors
 */
class WeatherClientException : public std::runtime_error {
public:
    explicit WeatherClientException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * RAII wrapper for jansson json_t objects
 */
class JsonPtr {
public:
    explicit JsonPtr(json_t* ptr = nullptr) : ptr_(ptr) {}

    ~JsonPtr() {
        if (ptr_) {
            json_decref(ptr_);
        }
    }

    // Move semantics
    JsonPtr(JsonPtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    JsonPtr& operator=(JsonPtr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                json_decref(ptr_);
            }
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // Delete copy operations
    JsonPtr(const JsonPtr&) = delete;
    JsonPtr& operator=(const JsonPtr&) = delete;

    json_t* get() const { return ptr_; }
    json_t* release() {
        json_t* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    explicit operator bool() const { return ptr_ != nullptr; }

private:
    json_t* ptr_;
};

/**
 * Configuration for WeatherClient
 */
struct ClientConfig {
    std::string host = "localhost";
    int port = 10680;
    int timeout_ms = 5000;

    ClientConfig() = default;
    ClientConfig(const std::string& h, int p) : host(h), port(p) {}
};

/**
 * Main weather client class using OOP principles
 */
class WeatherClient {
public:
    /**
     * Constructor with configuration
     * @throws WeatherClientException if initialization fails
     */
    explicit WeatherClient(const ClientConfig& config = ClientConfig());

    /**
     * Destructor - automatically cleans up resources (RAII)
     */
    ~WeatherClient();

    // Delete copy operations (non-copyable)
    WeatherClient(const WeatherClient&) = delete;
    WeatherClient& operator=(const WeatherClient&) = delete;

    // Move operations
    WeatherClient(WeatherClient&& other) noexcept;
    WeatherClient& operator=(WeatherClient&& other) noexcept;

    /**
     * Get current weather by coordinates
     * @param lat Latitude
     * @param lon Longitude
     * @return JSON response wrapped in JsonPtr
     * @throws WeatherClientException on error
     */
    JsonPtr getCurrentWeather(double lat, double lon);

    /**
     * Get weather by city name
     * @param city City name
     * @param country Optional country code
     * @param region Optional region name
     * @return JSON response wrapped in JsonPtr
     * @throws WeatherClientException on error
     */
    JsonPtr getWeatherByCity(const std::string& city,
                             const std::optional<std::string>& country = std::nullopt,
                             const std::optional<std::string>& region = std::nullopt);

    /**
     * Search for cities by query
     * @param query Search query (minimum 2 characters)
     * @return JSON response wrapped in JsonPtr
     * @throws WeatherClientException on error
     */
    JsonPtr searchCities(const std::string& query);

    /**
     * Get homepage content
     * @return JSON response wrapped in JsonPtr
     * @throws WeatherClientException on error
     */
    JsonPtr getHomepage();

    /**
     * Echo test endpoint
     * @return JSON response wrapped in JsonPtr
     * @throws WeatherClientException on error
     */
    JsonPtr echo();

    /**
     * Clear the client cache
     */
    void clearCache();

    /**
     * Set request timeout
     * @param timeout_ms Timeout in milliseconds
     */
    void setTimeout(int timeout_ms);

    /**
     * Get current configuration
     */
    const ClientConfig& getConfig() const { return config_; }

private:
    /**
     * Private implementation class (Pimpl idiom for hiding C dependencies)
     */
    class Impl;

    ClientConfig config_;
    std::unique_ptr<Impl> pimpl_;

    /**
     * Helper method to build cache keys
     */
    static std::string buildCacheKey(const std::string& endpoint,
                                     const std::string& params);

    /**
     * Helper method to make HTTP requests with caching
     */
    JsonPtr makeRequest(const std::string& url,
                       const std::string& cache_key);
};

} // namespace weather

#include "weather_client.hpp"

// C library headers
extern "C" {
#include "../network/http_client.h"
#include "../utils/client_cache.h"
#include "../utils/utils.h"
}

#include <cstdio>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace weather {

// Use constants from C headers:
// CACHE_MAX_ENTRIES and CACHE_DEFAULT_TTL are defined in client_cache.h

/**
 * Private implementation class (Pimpl idiom)
 * Hides C dependencies from the header
 */
class WeatherClient::Impl {
public:
    HttpClient* http_client = nullptr;
    ClientCache* cache = nullptr;

    explicit Impl(int timeout_ms) {
        http_client = http_client_create(timeout_ms);
        if (!http_client) {
            throw WeatherClientException("Failed to create HTTP client");
        }

        cache = client_cache_create(CACHE_MAX_ENTRIES, CACHE_DEFAULT_TTL);
        if (!cache) {
            http_client_destroy(http_client);
            throw WeatherClientException("Failed to create cache");
        }
    }

    ~Impl() {
        if (http_client) {
            http_client_destroy(http_client);
        }
        if (cache) {
            client_cache_destroy(cache);
        }
    }

    // Delete copy operations
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;
};

// WeatherClient implementation

WeatherClient::WeatherClient(const ClientConfig& config)
    : config_(config)
    , pimpl_(std::make_unique<Impl>(config.timeout_ms)) {
}

WeatherClient::~WeatherClient() = default;

// Move constructor
WeatherClient::WeatherClient(WeatherClient&& other) noexcept
    : config_(std::move(other.config_))
    , pimpl_(std::move(other.pimpl_)) {
}

// Move assignment
WeatherClient& WeatherClient::operator=(WeatherClient&& other) noexcept {
    if (this != &other) {
        pimpl_ = std::move(other.pimpl_);
        config_ = std::move(other.config_);
    }
    return *this;
}

std::string WeatherClient::buildCacheKey(const std::string& endpoint,
                                        const std::string& params) {
    return endpoint + ":" + params;
}

JsonPtr WeatherClient::makeRequest(const std::string& url,
                                   const std::string& cache_key) {
    // Check cache first
    char* cached = client_cache_get(pimpl_->cache, cache_key.c_str());
    if (cached) {
        json_error_t json_err;
        json_t* result = json_loads(cached, 0, &json_err);
        free(cached);

        if (result) {
            return JsonPtr(result);
        }
    }

    // Make HTTP request
    char* error = nullptr;
    if (http_client_get(pimpl_->http_client, url.c_str(), &error) != 0) {
        std::string error_msg = error ? error : "HTTP request failed";
        if (error) {
            free(error);
        }
        throw WeatherClientException(error_msg);
    }

    const char* body = http_client_get_body(pimpl_->http_client);
    if (!body) {
        throw WeatherClientException("Empty response from server");
    }

    // Parse JSON
    json_error_t json_err;
    json_t* result = json_loads(body, 0, &json_err);
    if (!result) {
        std::string error_msg = "JSON parse error: ";
        error_msg += json_err.text;
        throw WeatherClientException(error_msg);
    }

    // Check for API error response
    json_t* success_field = json_object_get(result, "success");
    if (success_field && json_is_boolean(success_field)) {
        if (!json_boolean_value(success_field)) {
            json_t* error_obj = json_object_get(result, "error");
            std::string error_msg = "API error";

            if (error_obj) {
                json_t* msg = json_object_get(error_obj, "message");
                if (msg && json_is_string(msg)) {
                    error_msg = json_string_value(msg);
                }
            }

            json_decref(result);
            throw WeatherClientException(error_msg);
        }
    }

    // Cache the successful response
    client_cache_set(pimpl_->cache, cache_key.c_str(), body);

    return JsonPtr(result);
}

JsonPtr WeatherClient::getCurrentWeather(double lat, double lon) {
    if (!validate_latitude(lat) || !validate_longitude(lon)) {
        throw WeatherClientException("Invalid coordinates");
    }

    std::ostringstream url;
    url << "http://" << config_.host << ":" << config_.port
        << "/v1/current?lat=" << std::fixed << std::setprecision(4) << lat
        << "&lon=" << std::fixed << std::setprecision(4) << lon;

    std::ostringstream params;
    params << "lat=" << std::fixed << std::setprecision(4) << lat
           << ":lon=" << std::fixed << std::setprecision(4) << lon;

    std::string cache_key = buildCacheKey("current", params.str());
    return makeRequest(url.str(), cache_key);
}

JsonPtr WeatherClient::getWeatherByCity(const std::string& city,
                                        const std::optional<std::string>& country,
                                        const std::optional<std::string>& region) {
    if (!validate_city_name(city.c_str())) {
        throw WeatherClientException("Invalid city name");
    }

    char* city_encoded = url_encode(city.c_str());
    if (!city_encoded) {
        throw WeatherClientException("Failed to encode city name");
    }

    std::ostringstream url;
    url << "http://" << config_.host << ":" << config_.port
        << "/v1/weather?city=" << city_encoded;

    free(city_encoded);

    if (country.has_value() && !country->empty()) {
        char* country_encoded = url_encode(country->c_str());
        if (country_encoded) {
            url << "&country=" << country_encoded;
            free(country_encoded);
        }
    }

    if (region.has_value() && !region->empty()) {
        char* region_encoded = url_encode(region->c_str());
        if (region_encoded) {
            url << "&region=" << region_encoded;
            free(region_encoded);
        }
    }

    // Normalize strings for cache key
    char normalized_city[256] = "";
    char normalized_country[256] = "";
    char normalized_region[256] = "";

    normalize_string_for_cache(city.c_str(), normalized_city, sizeof(normalized_city));

    if (country.has_value()) {
        normalize_string_for_cache(country->c_str(), normalized_country,
                                  sizeof(normalized_country));
    }

    if (region.has_value()) {
        normalize_string_for_cache(region->c_str(), normalized_region,
                                  sizeof(normalized_region));
    }

    std::ostringstream params;
    params << "city=" << normalized_city
           << ":country=" << normalized_country
           << ":region=" << normalized_region;

    std::string cache_key = buildCacheKey("weather", params.str());
    return makeRequest(url.str(), cache_key);
}

JsonPtr WeatherClient::searchCities(const std::string& query) {
    if (query.length() < 2) {
        throw WeatherClientException("Query must be at least 2 characters");
    }

    char* query_encoded = url_encode(query.c_str());
    if (!query_encoded) {
        throw WeatherClientException("Failed to encode query");
    }

    std::ostringstream url;
    url << "http://" << config_.host << ":" << config_.port
        << "/v1/cities?query=" << query_encoded;

    free(query_encoded);

    // Normalize query for cache key
    char normalized_query[256];
    normalize_string_for_cache(query.c_str(), normalized_query,
                              sizeof(normalized_query));

    std::ostringstream params;
    params << "query=" << normalized_query;

    std::string cache_key = buildCacheKey("cities", params.str());
    return makeRequest(url.str(), cache_key);
}

JsonPtr WeatherClient::getHomepage() {
    std::ostringstream url;
    url << "http://" << config_.host << ":" << config_.port << "/";

    std::string cache_key = buildCacheKey("homepage", "");
    return makeRequest(url.str(), cache_key);
}

JsonPtr WeatherClient::echo() {
    std::ostringstream url;
    url << "http://" << config_.host << ":" << config_.port << "/echo";

    char* error = nullptr;
    if (http_client_get(pimpl_->http_client, url.str().c_str(), &error) != 0) {
        std::string error_msg = error ? error : "HTTP request failed";
        if (error) {
            free(error);
        }
        throw WeatherClientException(error_msg);
    }

    const char* body = http_client_get_body(pimpl_->http_client);
    if (!body) {
        throw WeatherClientException("Empty response");
    }

    json_t* result = json_object();
    json_object_set_new(result, "echo", json_string(body));

    return JsonPtr(result);
}

void WeatherClient::clearCache() {
    if (pimpl_ && pimpl_->cache) {
        client_cache_clear(pimpl_->cache);
    }
}

void WeatherClient::setTimeout(int timeout_ms) {
    config_.timeout_ms = timeout_ms;
}

} // namespace weather

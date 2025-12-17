#include "api/weather_client.hpp"
#include "cli/cli.hpp"

#include <iostream>

enum class ExitCode {
    Ok = 0,
    InvalidArgs = 1,
    NetworkError = 2,
    ServerError = 3
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        CLI::printUsageStatic(argv[0]);
        return static_cast<int>(ExitCode::InvalidArgs);
    }

    try {
        WeatherClient client{"localhost", 10680};
        CLI cli{client};

        std::string cmd = argv[1];
        if (cmd == "interactive" || cmd == "-i") {
            cli.runInteractive();
            return static_cast<int>(ExitCode::Ok);
        }

        int rc = cli.runCommandLine(argc, argv);
        if (rc == static_cast<int>(ExitCode::InvalidArgs)) {
            cli.printUsage(argv[0]);
        }
        return rc;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return static_cast<int>(ExitCode::NetworkError);
    }
}

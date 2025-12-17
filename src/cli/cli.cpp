#include "cli.hpp"
#include "command.hpp"
#include "command_parser.hpp"

#include <iostream>
#include <sstream>
#include <vector>

CLI::CLI(WeatherClient& client)
    : client_(client) {}

void CLI::printUsage(const std::string& p) const {
    printUsageStatic(p);
}

void CLI::printUsageStatic(const std::string& p) {
    std::cout <<
        "Just Weather Client\n\n"
        "Usage:\n"
        "  " << p << " current <lat> <lon>\n"
        "  " << p << " interactive\n";
}

static std::vector<std::string> split(const std::string& s) {
    std::istringstream iss{s};
    std::vector<std::string> tokens;
    std::string t;
    while (iss >> t) tokens.push_back(t);
    return tokens;
}

void CLI::runInteractive() {
    std::string line;

    std::cout << "Just Weather Interactive Client\n";

    while (true) {
        std::cout << "just-weather> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "quit" || line == "exit") break;
        if (line == "help") {
            printUsage("just-weather");
            continue;
        }

        try {
            auto tokens = split(line);
            auto cmd = CommandParser::parse(client_, tokens);
            cmd->execute();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

int CLI::runCommandLine(int argc, char* argv[]) {
    std::vector<std::string> tokens;
    for (int i = 1; i < argc; ++i)
        tokens.emplace_back(argv[i]);

    try {
        auto cmd = CommandParser::parse(client_, tokens);
        cmd->execute();
        return 0;
    }
    catch (const std::invalid_argument&) {
        return 1;
    }
    catch (...) {
        return 3;
    }
}

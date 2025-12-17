#include "cli.hpp"
#include "command.hpp"
#include "command_parser.hpp"

#include <iostream>
#include <sstream>
#include <vector>

CLI::CLI(weather::WeatherClient& client)
    : client_(client) {}

void CLI::printUsage(const std::string& p) const {
    printUsageStatic(p);
}

void CLI::printUsageStatic(const std::string& p) {
    std::cout <<
        "Just Weather Client\n\n"
        "Usage:\n"
        "  " << p << " current <lat> <lon>\n"
        "  " << p << " weather <city> [country] [region]\n"
        "  " << p << " cities <query>\n"
        "  " << p << " homepage\n"
        "  " << p << " echo\n"
        "  " << p << " clear-cache\n"
        "  " << p << " interactive    # Enter interactive mode\n\n"
        "Examples:\n"
        "  " << p << " current 59.33 18.07\n"
        "  " << p << " weather Stockholm SE\n"
        "  " << p << " cities Stock\n"
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
    std::cout << "Connected to: localhost:10680\n";
    std::cout << "Type 'help' for commands, 'quit' to exit\n\n";

    while (true) {
        std::cout << "just-weather> ";
        std::cout.flush();

        if (!std::getline(std::cin, line)) break;

        if (line.empty()) continue;

        if (line == "quit" || line == "q" || line == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }

        if (line == "help") {
            std::cout << "\nAvailable commands:\n";
            std::cout << "  current <lat> <lon>             - Get current weather by coordinates\n";
            std::cout << "  weather <city> [country]        - Get weather by city name\n";
            std::cout << "  cities <query>                  - Search for cities\n";
            std::cout << "  homepage                        - Get API homepage\n";
            std::cout << "  echo                            - Test echo endpoint\n";
            std::cout << "  clear-cache                     - Clear client cache\n";
            std::cout << "  help                            - Show this help\n";
            std::cout << "  quit / exit / q                 - Exit interactive mode\n\n";
            std::cout << "Examples:\n";
            std::cout << "  current 59.33 18.07\n";
            std::cout << "  weather Kyiv UA\n";
            std::cout << "  cities London\n\n";
            continue;
        }

        try {
            auto tokens = split(line);
            if (!tokens.empty()) {
                auto cmd = CommandParser::parse(client_, tokens);
                cmd->execute();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
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

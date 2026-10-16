#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

namespace {

void usage() {
    std::cout << "Usage: kyo {install|update|remove|search|clean} [arguments...]\n"
                 "\n"
                 "Commands:\n"
                 "  install, i   <package...>   Install packages\n"
                 "  update, u                    Update the system\n"
                 "  remove, r    <package...>   Remove packages (with -o)\n"
                 "  search, s    <term...>      Search for packages\n"
                 "  clean, c                     Clean orphans and obsolete packages\n";
}

std::string joinArguments(const std::vector<std::string> &arguments) {
    std::string joined;
    for (const auto &argument : arguments) {
        if (!joined.empty()) {
            joined += ' ';
        }
        joined += argument;
    }
    return joined;
}

int runCommand(const std::vector<std::string> &arguments) {
    std::vector<char *> argv;
    argv.reserve(arguments.size() + 1);
    for (const auto &argument : arguments) {
        argv.push_back(const_cast<char *>(argument.c_str()));
    }
    argv.push_back(nullptr);

    pid_t processId;
    const int spawnError = posix_spawnp(&processId, argv[0], nullptr, nullptr, argv.data(), environ);
    if (spawnError != 0) {
        std::cerr << "kyo: unable to run " << arguments[0] << ": "
                  << std::strerror(spawnError) << '\n';
        return 1;
    }

    int status;
    while (waitpid(processId, &status, 0) == -1) {
        if (errno == EINTR) {
            continue;
        }
        std::cerr << "kyo: waitpid failed: " << std::strerror(errno) << '\n';
        return 1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int runXbps(const std::vector<std::string> &arguments) {
    std::vector<std::string> command{"sudo"};
    command.insert(command.end(), arguments.begin(), arguments.end());
    return runCommand(command);
}

bool isCommand(const std::string &command, const char *longName, const char *shortName) {
    return command == longName || command == shortName;
}

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 1;
    }

    const std::string command = argv[1];
    std::vector<std::string> arguments;
    for (int index = 2; index < argc; ++index) {
        arguments.emplace_back(argv[index]);
    }

    if (isCommand(command, "install", "i")) {
        std::cout << "Installing " << joinArguments(arguments) << "...\n";
        std::vector<std::string> xbpsArguments{"xbps-install", "-S"};
        xbpsArguments.insert(xbpsArguments.end(), arguments.begin(), arguments.end());
        return runXbps(xbpsArguments);
    }
    if (isCommand(command, "update", "u")) {
        std::cout << "Updating...\n";
        return runXbps({"xbps-install", "-Syu"});
    }
    if (isCommand(command, "remove", "r")) {
        std::cout << "Removing " << joinArguments(arguments) << "...\n";
        std::vector<std::string> xbpsArguments{"xbps-remove", "-o"};
        xbpsArguments.insert(xbpsArguments.end(), arguments.begin(), arguments.end());
        return runXbps(xbpsArguments);
    }
    if (isCommand(command, "search", "s")) {
        std::cout << "searching in deepweb " << joinArguments(arguments) << "...\n";
        std::vector<std::string> xbpsArguments{"xbps-query", "-Rs"};
        xbpsArguments.insert(xbpsArguments.end(), arguments.begin(), arguments.end());
        return runXbps(xbpsArguments);
    }
    if (isCommand(command, "clean", "c")) {
        std::cout << "Cleaning...\n";
        return runXbps({"xbps-remove", "-oO"});
    }

    std::cerr << "Unknown command: " << command << '\n';
    usage();
    return 1;
}
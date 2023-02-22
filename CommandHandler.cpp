#include "CommandHandler.h"
#include "ValueParser.h"
#include <iostream>
namespace ParticleLife {
    void CommandHandler::printCommandUsage(const std::string& commandName) const
    {
        std::cout << commandName;
        for (auto&& arg : commands_.at(commandName)->getArguments()) {
            std::cout << " <" << arg << ">";
        }
        std::cout << std::endl;
    }

    CommandHandler::CommandHandler(Options& options) : commands_(), options_(options) {}

    void CommandHandler::registerCommand(std::string&& commandName, std::unique_ptr<Command>&& command)
    {
        commands_.emplace(std::move(commandName), std::move(command));
    }

    void CommandHandler::handleCommand(const std::vector<std::string>& args) const
    {
        if (args.size() == 0)
            return;
        size_t argc = args.size() - 1;
        const std::string& commandName = args[0];
        if (commands_.contains(commandName)) {
            const Command& cmd = *commands_.at(commandName);
            if (argc == cmd.argCount()) {
                if (!cmd.run(options_, args)) {
                    printCommandUsage(commandName);
                }
            }
            else if (argc == 0) {
                cmd.printCurrentSettings(options_);
                std::cout << "  -- Command Usage: --" << std::endl;
                std::cout << commandName << ": ";
                cmd.printCommandDescription();
                printCommandUsage(commandName);
            }
            else {
                std::cout << ERROR_TAG << "Incorrect number of arguments (" << argc << "), expected " << cmd.argCount() << "." << std::endl;
                printCommandUsage(commandName);
            }
        }
        else {
            std::cout << ERROR_TAG << "Invalid command, use \"help\" for help." << std::endl;
        }
    }

    const std::map<std::string, std::unique_ptr<Command>>& CommandHandler::getCommands() const
    {
        return commands_;
    }
}
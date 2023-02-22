#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_
#include <map>
#include <string>
#include <memory>
#include "Command.h"
#include <vector>
namespace ParticleLife {
    /// @brief holds and helps executing registered commands
    class CommandHandler {
    private:
        Options& options_;
        /// @brief holds registered commands indexed by their name
        std::map<std::string, std::unique_ptr<Command>> commands_;
        /// @brief print information about command usage
        /// @param commandName name of the command, expected to be valid
        void printCommandUsage(const std::string& commandName) const;
    public:
        /// @param options reference to the options to which the commands apply
        CommandHandler(Options& options);
        /// @brief register a command
        /// @param commandName name to register the command under
        /// @param command unique pointer with the command instance
        void registerCommand(std::string&& commandName, std::unique_ptr<Command>&& command);
        /// @brief run the command defined by a vector of strings
        /// @param args vector of strings, first denoting the command name and the others its arguments
        void handleCommand(const std::vector<std::string>& args) const;
        /// @brief get map of all registered commands indexed by their name
        /// @return const reference to the map
        const std::map<std::string, std::unique_ptr<Command>>& getCommands() const;
    };
}
#endif
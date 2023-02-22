#ifndef COMMAND_H_
#define COMMAND_H_
#include "ValueParser.h"
#include "Options.h"
#include <vector>
#include <string>

namespace ParticleLife {
    /// @brief represents a command the user can execute
    class Command {
    protected:
        ValueParser parser_;
        /// @brief print a table of float values
        /// @param values the values to print
        void Tabulate(std::vector<std::vector<float>>&& values) const;
    public:
        inline Command() : parser_() {}
        /// @brief how many arguments does this command have
        /// @return the number of arguments
        inline virtual size_t argCount() const = 0;
        /// @brief parse the arguments and run the command
        /// @param options reference to options to modify
        /// @param args command arguments as strings, the count is expected be correct (equal to argCount())
        /// @return did the command succeed
        virtual bool run(Options& options, const std::vector<std::string>& args) const = 0;
        /// @brief print the corresponding current settings
        /// @param options reference to options to read from
        inline virtual void printCurrentSettings(const Options& options) const {}
        /// @brief print the command description
        virtual void printCommandDescription() const = 0;
        /// @brief get the names of this command's arguments
        /// @return vector of the names
        inline virtual std::vector<std::string> getArguments() const { return {}; }
    };

    class FPSCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class TPSCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class TimeStepCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class SimSpeedCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class WorldSizeCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class FrictionCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class ParticleRadiusCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class RepulsionCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class ChunkCountCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class AddSpeciesCommand : public Command {
        inline size_t argCount() const override { return 1; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class ParticleCountCommand : public Command {
        inline size_t argCount() const override { return 2; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class ColorCommand : public Command {
        inline size_t argCount() const override { return 4; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class AttractionCommand : public Command {
        inline size_t argCount() const override { return 3; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class AttractionRangeCommand : public Command {
        inline size_t argCount() const override { return 3; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class RepulsionRangeCommand : public Command {
        inline size_t argCount() const override { return 3; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCurrentSettings(const Options& options) const override;
        void printCommandDescription() const override;
        std::vector<std::string> getArguments() const override;
    };
    class PauseCommand : public Command {
        inline size_t argCount() const override { return 0; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCommandDescription() const override;
    };
    class StepCommand : public Command {
        inline size_t argCount() const override { return 0; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCommandDescription() const override;
    };
    class ExitCommand : public Command {
        inline size_t argCount() const override { return 0; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCommandDescription() const override;
    };
    class HelpCommand : public Command {
    private:
        const std::map<std::string, std::unique_ptr<Command>>& commands_;
    public:
        /// @param reference to map of command names to commands
        inline HelpCommand(const std::map<std::string, std::unique_ptr<Command>>& commands) : Command(), commands_(commands) {}
        inline size_t argCount() const override { return 0; }
        bool run(Options& options, const std::vector<std::string>& args) const override;
        void printCommandDescription() const override;
    };
}
#endif
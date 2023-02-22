#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "Options.h"
#include "CommandHandler.h"
namespace ParticleLife {
    /// @brief reads user commands from stdin and hands them over to a CommandHandler to execute
    class InputHandler {
    private:
        Options& options_;
        CommandHandler commandHandler_;
        /// @brief is stdin non-empty
        bool stdinHasData() const;
    public:
        InputHandler(Options& options);
        /// @brief register commands and prepare for user input
        void init();
        /// @brief check wheter input is ready an if so, handle it
        void pollInputs() const;
    };
}
#endif
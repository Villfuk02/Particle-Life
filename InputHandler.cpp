#include "InputHandler.h"
#include <iostream>
#include <sstream>
namespace ParticleLife {
    constexpr char PROMPT[] = "> ";

    InputHandler::InputHandler(Options& options) : commandHandler_(options), options_(options) {}

    //taken from https://stackoverflow.com/a/71992965
#if defined(__GNUG__) || defined(__GNUC__)
#define OS_LINUX
#elif defined(_MAC)
#define OS_MAC
#elif defined(_WIN32)
#define OS_WIN
#endif
#ifdef OS_WIN
#include <conio.h>
#elif defined(OS_LINUX) || defined(OS_MAC)
#include <unistd.h>
#include <sys/socket.h>
#endif
    bool InputHandler::stdinHasData() const
    {
#   if defined(OS_WIN)
        // this works by harnessing Windows' black magic:
        return _kbhit();
#   elif defined(OS_LINUX) || defined(OS_MAC) 
        // using a timeout of 0 so we aren't waiting:
        struct timespec timeout { 0l, 0l };

        // create a file descriptor set
        fd_set fds{};

        // initialize the fd_set to 0
        FD_ZERO(&fds);
        // set the fd_set to target file descriptor 0 (STDIN)
        FD_SET(0, &fds);

        // pselect the number of file descriptors that are ready, since
        //  we're only passing in 1 file descriptor, it will return either
        //  a 0 if STDIN isn't ready, or a 1 if it is.
        return pselect(0 + 1, &fds, nullptr, nullptr, &timeout, nullptr) == 1;
#   else
        // throw a compiler error
        static_assert(false, "Failed to detect a supported operating system!");
#   endif
    }

    void InputHandler::init() {
        commandHandler_.registerCommand("help", std::make_unique<HelpCommand>(commandHandler_.getCommands()));
        commandHandler_.registerCommand("fps", std::make_unique<FPSCommand>());
        commandHandler_.registerCommand("tps", std::make_unique<TPSCommand>());
        commandHandler_.registerCommand("ts", std::make_unique<TimeStepCommand>());
        commandHandler_.registerCommand("ss", std::make_unique<SimSpeedCommand>());
        commandHandler_.registerCommand("ws", std::make_unique<WorldSizeCommand>());
        commandHandler_.registerCommand("f", std::make_unique<FrictionCommand>());
        commandHandler_.registerCommand("dr", std::make_unique<ParticleRadiusCommand>());
        commandHandler_.registerCommand("r", std::make_unique<RepulsionCommand>());
        commandHandler_.registerCommand("cc", std::make_unique<ChunkCountCommand>());
        commandHandler_.registerCommand("add", std::make_unique<AddSpeciesCommand>());
        commandHandler_.registerCommand("sc", std::make_unique<ParticleCountCommand>());
        commandHandler_.registerCommand("dc", std::make_unique<ColorCommand>());
        commandHandler_.registerCommand("sa", std::make_unique<AttractionCommand>());
        commandHandler_.registerCommand("sar", std::make_unique<AttractionRangeCommand>());
        commandHandler_.registerCommand("srr", std::make_unique<RepulsionRangeCommand>());
        commandHandler_.registerCommand("p", std::make_unique<PauseCommand>());
        commandHandler_.registerCommand("s", std::make_unique<StepCommand>());
        commandHandler_.registerCommand("q", std::make_unique<ExitCommand>());

        std::cout << PROMPT;
    }
    void InputHandler::pollInputs() const
    {
        if (!stdinHasData())
            return;
        std::string input;
        if (!std::getline(std::cin, input)) {
            exit(0);
        }
        if (input.empty()) {
            std::cout << PROMPT;
            return;
        }
        std::istringstream stream(input);
        std::vector<std::string> args;
        while (!stream.eof()) {
            std::string arg;
            stream >> arg;
            if (!arg.empty())
                args.push_back(std::move(arg));
        }
        commandHandler_.handleCommand(args);
        std::cout << PROMPT;
    }
}
#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H
#include "Options.h"
#include "Simulation.h"
#include "Renderer.h"
#include "InputHandler.h"
#include <chrono>
namespace ParticleLife {
    class ProgramManager {
    private:
        Options options_;
        Simulation simulation_;
        Renderer renderer_;
        InputHandler inputHandler_;
        std::chrono::steady_clock::time_point startTime_;
        /// @brief planned time of last simulation step
        double lastTick_;
        /// @brief planned time of last frame
        double lastFrame_;
        /// @brief real time of last simulation step
        double realLastTick_;
        /// @brief real time of last frame
        double realLastFrame_;
        /// @brief average fps
        double fps_;
        /// @brief average tps
        double tps_;
        /// @brief initialize all components
        void init();
        /// @brief calls to simulate steps, render frames and poll inputs at appropriate intervals
        void mainLoop();
        double getTimeSinceStart() const;
        void tick(double time);
        void renderFrame(double time);
        void pollInputs();
    public:
        ProgramManager();
        /// @brief initializes program and starts it
        void run();
    };
}
#endif
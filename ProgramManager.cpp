#include <iostream>
#include "ProgramManager.h"
#include <algorithm>
namespace ParticleLife {
    constexpr size_t FRAME_DROP_THRESHOLD = 4;
    constexpr size_t TICK_DROP_THRESHOLD = 10;
    constexpr double RATE_DISPLAY_SMOOTHING = 0.25;

    constexpr double NANOSECONDS_PER_SECOND = 1000000000.0;
    constexpr char ONE_DECIMAL[] = "{:.1f}";

    ProgramManager::ProgramManager() :
        options_(), simulation_(options_), renderer_(options_), inputHandler_(options_), startTime_(),
        lastFrame_(0), lastTick_(0), realLastFrame_(0), realLastTick_(0), fps_(1 / options_.getFrameTime()), tps_(1 / options_.getRealTimeStep()) {}

    void ProgramManager::run() {
        init();
        mainLoop();
    }

    void ProgramManager::init() {
        startTime_ = std::chrono::steady_clock::now();
        simulation_.init();
        renderer_.init();
        inputHandler_.init();
    }

    void ProgramManager::mainLoop() {
        while (true) {
            double time = getTimeSinceStart();
            if ((!options_.paused && time - lastTick_ >= options_.getRealTimeStep()) || options_.step) {
                options_.step = false;
                tick(time);
            }
            if (time - lastFrame_ >= options_.getFrameTime())
            {
                renderFrame(time);
            }
            pollInputs();
        }
    }

    double ProgramManager::getTimeSinceStart() const {
        return (std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - startTime_).count()) / NANOSECONDS_PER_SECOND;
    }

    void ProgramManager::tick(double time) {
        if (time - lastTick_ >= options_.getRealTimeStep() * TICK_DROP_THRESHOLD)
            lastTick_ = time - options_.getRealTimeStep() * (TICK_DROP_THRESHOLD - 1);
        else
            lastTick_ += options_.getRealTimeStep();

        double ellapsed = time - realLastTick_;
        double clamped = std::min(ellapsed, RATE_DISPLAY_SMOOTHING);
        tps_ = (tps_ * (RATE_DISPLAY_SMOOTHING - clamped) + clamped / ellapsed) / RATE_DISPLAY_SMOOTHING;
        realLastTick_ = time;

        simulation_.tick();
    }
    void ProgramManager::renderFrame(double time) {
        if (time - lastFrame_ >= options_.getFrameTime() * FRAME_DROP_THRESHOLD)
            lastFrame_ = time - options_.getFrameTime() * (FRAME_DROP_THRESHOLD - 1);
        else
            lastFrame_ += options_.getFrameTime();

        double ellapsed = time - realLastFrame_;
        double clamped = std::min(ellapsed, RATE_DISPLAY_SMOOTHING);
        fps_ = (fps_ * (RATE_DISPLAY_SMOOTHING - clamped) + clamped / ellapsed) / RATE_DISPLAY_SMOOTHING;
        realLastFrame_ = time;

        renderer_.handleEvents();

        renderer_.clear();
        renderer_.renderParticles(simulation_.getParticles());
        renderer_.renderText(0, "Ellapsed: " + std::format(ONE_DECIMAL, simulation_.getTime()) + "s", false);
        if (options_.paused)
            renderer_.renderText(0, "PAUSED", true);
        else
            renderer_.renderText(0, std::format(ONE_DECIMAL, tps_) + "/" + std::format(ONE_DECIMAL, 1 / options_.getRealTimeStep()) + " tps", true);
        renderer_.renderText(1, std::format(ONE_DECIMAL, fps_) + " fps", true);
        renderer_.display();
    }
    void ProgramManager::pollInputs() {
        inputHandler_.pollInputs();
    }
}
#ifndef RENDERER_H
#define RENDERER_H
#include "Options.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
namespace ParticleLife {
    class Renderer {
    private:
        Options& options_;
        sf::RenderWindow window_;
        sf::Font font_;
        sf::Text text_;
        void renderParticlesAsPoints(const std::vector<std::vector<Particle>>& particles, float pixelsPerUnit, sf::Vector2f windowCenterOffset);
        void renderParticlesAsCircles(const std::vector<std::vector<Particle>>& particles, float pixelsPerUnit, sf::Vector2f windowCenterOffset);
    public:
        Renderer(Options& options);
        /// @brief initialize objects
        void init();
        /// @brief handle window events
        void handleEvents();
        /// @brief clear current frame buffer
        void clear();
        /// @brief draw particles onto current frame buffer
        /// @param particles collection of particles of each particle species
        void renderParticles(const std::vector<std::vector<Particle>>& particles);
        /// @brief draw text onto current frame buffer
        /// @param line line of text from the top of the stream
        /// @param text text to display
        /// @param alignRight align text to the right of the window
        void renderText(size_t line, std::string text, bool alignRight);
        /// @brief swap frame buffers (display new frame)
        void display();
    };
}
#endif
#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Graphics.hpp>
namespace ParticleLife {
    class Particle {
    private:
        sf::Vector2f position_;
        sf::Vector2f velocity_;
        sf::Vector2f acceleration_;
    public:
        Particle();
        Particle(sf::Vector2f position);
        /// @brief update particle values by simulation one time step
        void tick(float timeStep, float worldSize, float dragMultiplier);
        /// @brief modify this particle's acceleration 
        void addForce(sf::Vector2f acceleration);
        /// @brief get this particle's position
        sf::Vector2f getPosition() const;
    };
}
#endif
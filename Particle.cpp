#include <iostream>
#include "Particle.h"
namespace ParticleLife {
    Particle::Particle() : position_(), velocity_(), acceleration_() {}

    Particle::Particle(sf::Vector2f position) :position_(position), velocity_(), acceleration_() {}

    void Particle::tick(float timeStep, float worldSize, float dragMultiplier) {
        velocity_ *= dragMultiplier;
        velocity_ += acceleration_ * timeStep;
        position_ += velocity_ * timeStep;
        position_.x = std::fmodf(position_.x + worldSize, worldSize);
        position_.y = std::fmodf(position_.y + worldSize, worldSize);
        acceleration_ = {}; // reset acceleration
    }

    void Particle::addForce(sf::Vector2f acceleration) {
        acceleration_ += acceleration;
    }

    sf::Vector2f Particle::getPosition() const {
        return position_;
    }
}
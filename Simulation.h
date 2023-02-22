#ifndef SIMULATION_H
#define SIMULATION_H
#include "Options.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>
namespace ParticleLife {
    class Simulation {
    private:
        Options& options_;
        /// @brief time simulated
        double simTime_;
        /// @brief particles split by species
        std::vector<std::vector<Particle>> particles_;
        /// @brief particles split by species, chunk x, chunk y
        std::vector<std::vector<std::vector<std::vector<Particle*>>>> chunks_;
        /// @brief create or destroy particles to match counts specified in options
        void updateParticleCounts();
        /// @brief assign particles to corresponding chunks
        void updateChunks();
        /// @brief update acceleration of each particle
        void updateParticleForces();
        /// @brief update velocity and position of each particle
        void updateParticlePositions();
        /// @brief add a particle of given species
        /// @param species species id
        void addParticle(size_t species);
        /// @brief get coordinates of the chunk containing this world position
        sf::Vector2u getChunk(sf::Vector2f pos);
        /// @brief update acceleration of each particle of given species within given chunk
        /// @param species species id
        /// @param chunkX horizontal chunk coordinate
        /// @param chunkY vertical chunk coordinate
        void updateChunk(size_t species, size_t chunkX, size_t chunkY);
        /// @brief update acceleration of each particle of given species within given chunk as affected by particles of other species within other given chunk
        /// @param species affected species id
        /// @param chunkX horizontal chunk coordinate
        /// @param chunkY vertical chunk coordinate
        /// @param otherSpecies other species id
        /// @param offsetX horizontal offset of other chunk compared to initial chunk
        /// @param offsetY vertical offset of other chunk compared to initial chunk
        void updateChunk(size_t species, size_t chunkX, size_t chunkY, size_t otherSpecies, size_t offsetX, size_t offsetY);
        /// @brief update acceleration of a given particle as affected by other particle
        /// @param particle particle to update
        /// @param species species of the given partcle
        /// @param otherPos position of the other particle
        /// @param otherSpecies species id of the other particle
        void updateParticle(Particle& particle, const ParticleSpecies& species, sf::Vector2f otherPos, size_t otherSpecies);
    public:
        Simulation(Options& options);
        /// @brief initialize simulation
        void init();
        /// @brief simulate one step of simulation
        void tick();
        /// @brief get time simulated
        inline double getTime() const { return simTime_; }
        /// @brief get particles split by species
        inline const std::vector<std::vector<Particle>>& getParticles() const { return particles_; }
    };
}
#endif
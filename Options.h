#ifndef OPTIONS_H
#define OPTIONS_H
#include <vector>
#include <random>
#include "ParticleSpecies.h"
#include <SFML/Graphics.hpp>
namespace ParticleLife {
    /// @brief stores current options
    class Options {
    private:
        /// @brief seconds between frames
        float frameTime_;
        /// @brief size of simulation steps in seconds
        float timeStep_;
        /// @brief simulation speed multiplier
        float simSpeed_;
        /// @brief timeStep / targetSimSpeed - seconds between simulation steps
        float realTimeStep_;
        /// @brief width and height
        float worldSize_;
        /// @brief portion of velocity left after one second
        float friction_;
        /// @brief portion of velocity left after one step
        float frictionMultiplierPerTick_;
        /// @brief display radius of particles
        float particleRadius_;
        /// @brief maximum repulsion strength (when two particles are on top of each other)
        float repulsion_;
        /// @brief number of chunks along each axis
        size_t chunkCount_;
        /// @brief width and height of one chunk
        float chunkSize_;
        /// @brief maximal of attraction ranges divided by chunk size
        float maxChunkRange_;
        /// @brief list of species
        std::vector<ParticleSpecies> species_;
        /// @brief chunk offsets sorted by distance from (0, 0) up to max chunk range
        std::vector<sf::Vector2u> chunkPattern_;

        /// @brief recalculate max chunk range, prepare chunk pattern and recaclculate chunk ranges of particle species
        void recalculateChunks();
        /// @brief prepare chunk pattern
        void recalculateChunkPattern();
        /// @brief recaclculate chunk ranges of particle species
        void recalculateChunkRanges();
        /// @brief generate random Color
        sf::Color getRandomColor();

        /// @brief calculate square distance from nearest point in (0, 0)
        class chunkDistanceSquared {
        public:
            size_t operator()(sf::Vector2u chunkPos);
        };
        /// @brief compare two chunks by distance from nearest point in (0, 0)
        class chunkDistanceComparer {
        public:
            bool operator()(sf::Vector2u a, sf::Vector2u b);
        };
    public:
        /// @brief random engine
        std::mt19937_64 random;
        /// @brief is the simulation paused
        bool paused;
        /// @brief should the (paused) simulation advance one step
        bool step;

        /// @brief sets options to their defaults
        Options();
        /// @brief set target frames per second
        void setFPS(float fps);
        /// @brief get target number of seconds between frames
        float getFrameTime() const;
        /// @brief set the size of simulation steps in seconds
        void setTimeStep(float timeStep);
        /// @brief get the size of simulation steps in seconds
        float getTimeStep() const;
        void setSimulationSpeed(float simulationSpeed);
        float getSimulationSpeed() const;
        /// @brief get time step between simulation steps
        float getRealTimeStep() const;
        /// @brief set world width and height
        void setWorldSize(float worldSize);
        /// @brief get world width and height
        float getWorldSize() const;
        /// @brief set portion of velocity left after one second
        void setFriction(float coefficient);
        /// @brief get portion of velocity left after one second
        float getFriction() const;
        /// @brief get portion of velocity left after one step
        float getFrictionMultiplierPerTick() const;
        /// @brief set display radius of the particle
        void setParticleRadius(float particleRadius);
        /// @brief get display radius of the particle
        float getParticleRadius() const;
        /// @brief set maximum repulsion strength (when two particles are on top of each other)
        void setRepulsion(float repulsion);
        /// @brief get maximum repulsion strength (when two particles are on top of each other)
        float getRepulsion() const;
        /// @brief set number of chunks along both axes
        void setChunkCount(size_t count);
        /// @brief get number of chunks along each axis
        size_t getChunkCount() const;
        /// @brief get width and height of each chunk
        float getChunkSize() const;
        /// @brief add a new particle species
        /// @param particleCount number of particles of this new species
        void addRandomSpecies(size_t particleCount);
        /// @brief get number of species
        size_t getSpeciesCount() const;
        /// @brief get particle species by id
        const ParticleSpecies& getSpecies(size_t id) const;
        /// @brief set number of particles of a given species
        /// @param id species id
        /// @param count particle count
        void setParticleCount(size_t id, size_t count);
        /// @brief set display color of particles of a given species
        /// @param id species id
        /// @param color display color
        void setSpeciesColor(size_t id, sf::Color color);
        /// @brief set how much are particles of one species attracted to particles of other species
        /// @param id species id
        /// @param other other species id
        /// @param attraction peak attraction strength (negative means repulsion)
        void setSpeciesAttraction(size_t id, size_t other, float attraction);
        /// @brief set from how far away are particles of one species attracted to particles of other species
        /// @param id species id
        /// @param other other species id
        /// @param range maximum distance
        void setSpeciesAttractionRange(size_t id, size_t other, float range);
        /// @brief set how close must a particle of one species be to a particle of other species to start being repelled
        /// @param id species id
        /// @param other other species id
        /// @param range maximum distance
        void setSpeciesRepulsionRange(size_t id, size_t other, float range);
        /// @brief get chunk offsets sorted by distance from (0, 0) up to max chunk range
        const std::vector<sf::Vector2u>& getChunkPattern() const;
    };
}
#endif
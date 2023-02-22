#ifndef PARTICLE_SPECIES_H
#define PARTICLE_SPECIES_H
#include <SFML/Graphics.hpp>
#include <vector>
namespace ParticleLife {
    class ParticleSpecies {
    public:
        /// @brief unique species id
        size_t id;
        /// @brief display color
        sf::Color color;
        /// @brief number of particles of this species
        size_t count;
        /// @brief how close must a particle of this species be to a particle of other species to start being repelled
        std::vector<float> repulsionRange;
        /// @brief how much are particles of this species attracted to particles of other species (at peak)
        std::vector<float> attraction;
        /// @brief from how far away are particles of this species attracted to particles of other species
        std::vector<float> attractionRange;
        /// @brief amount of closest chunks to check (in each direction) for particles which could be within attractionRange
        std::vector<size_t> chunkRange;
        ParticleSpecies();
        ParticleSpecies(size_t id_, const sf::Color& color_, size_t count_);
    };
}
#endif
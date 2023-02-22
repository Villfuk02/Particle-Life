#include "Simulation.h"
#include <iostream>
#include <execution>
#include <algorithm>
namespace ParticleLife {
    void Simulation::updateParticleCounts()
    {
        if (particles_.size() != options_.getSpeciesCount())
            particles_.resize(options_.getSpeciesCount());
        for (size_t s = 0; s < options_.getSpeciesCount(); s++)
        {
            if (particles_[s].size() > options_.getSpecies(s).count) {
                particles_[s].resize(options_.getSpecies(s).count);
            }
            else {
                while (particles_[s].size() < options_.getSpecies(s).count) {
                    addParticle(s);
                }
            }
        }
    }

    void Simulation::updateChunks() {
        if (chunks_.size() != options_.getSpeciesCount())
            chunks_.resize(options_.getSpeciesCount());
        for (auto&& s : chunks_)
        {
            if (s.size() != options_.getChunkCount())
                s.resize(options_.getChunkCount());
            for (auto&& x : s)
            {
                if (x.size() != options_.getChunkCount())
                    x.resize(options_.getChunkCount());
                for (auto&& y : x)
                {
                    y.clear();
                }
            }
        }
        for (size_t s = 0; s < particles_.size(); s++)
        {
            for (auto&& p : particles_[s]) {
                sf::Vector2u chunk = getChunk(p.getPosition());
                chunks_[s][chunk.x][chunk.y].push_back(&p);
            }
        }
    }

    void Simulation::updateParticleForces()
    {
        std::vector<size_t> ss = {};
        for (size_t i = 0; i < options_.getSpeciesCount(); i++)
        {
            ss.push_back(i);
        }
        std::for_each(
            std::execution::par,
            ss.begin(),
            ss.end(),
            [this](auto&& s)
            {
                for (size_t x = 0; x < options_.getChunkCount(); x++)
                {
                    for (size_t y = 0; y < options_.getChunkCount(); y++)
                    {
                        updateChunk(s, x, y);
                    }
                }
            });
    }

    void Simulation::updateParticlePositions()
    {
        for (auto&& s : particles_) {
            for (auto&& particle : s) {
                particle.tick(options_.getTimeStep(), options_.getWorldSize(), options_.getFrictionMultiplierPerTick());
            }
        }
    }

    void Simulation::addParticle(size_t species) {
        std::uniform_real_distribution<float> distribution(0, options_.getWorldSize());
        particles_[species].emplace_back(sf::Vector2f(distribution(options_.random), distribution(options_.random)));
    }

    sf::Vector2u Simulation::getChunk(sf::Vector2f pos) {
        sf::Vector2u chunk = sf::Vector2u((unsigned int)std::floorf(pos.x / options_.getChunkSize()), (unsigned int)std::floorf(pos.y / options_.getChunkSize()));
        if (chunk.x >= options_.getChunkCount())
            chunk.x = (unsigned int)options_.getChunkCount() - 1;
        if (chunk.y >= options_.getChunkCount())
            chunk.y = (unsigned int)options_.getChunkCount() - 1;
        return chunk;
    }

    void Simulation::updateChunk(size_t species, size_t chunkX, size_t chunkY) {
        for (size_t otherSpecies = 0; otherSpecies < options_.getSpeciesCount(); otherSpecies++) {
            updateChunk(species, chunkX, chunkY, otherSpecies, 0, 0);
            size_t chunkRange = options_.getSpecies(species).chunkRange[otherSpecies];
            for (size_t i = 0; i < chunkRange; i++)
            {
                sf::Vector2u offset = options_.getChunkPattern()[i];
                updateChunk(species, chunkX, chunkY, otherSpecies, offset.x, offset.y);
                updateChunk(species, chunkX, chunkY, otherSpecies, offset.y, options_.getChunkCount() - offset.x);
                updateChunk(species, chunkX, chunkY, otherSpecies, options_.getChunkCount() - offset.x, options_.getChunkCount() - offset.y);
                updateChunk(species, chunkX, chunkY, otherSpecies, options_.getChunkCount() - offset.y, offset.x);
            }
        }
    }
    void Simulation::updateChunk(size_t species, size_t chunkX, size_t chunkY, size_t otherSpecies, size_t offsetX, size_t offsetY) {
        for (auto&& particle : chunks_[species][chunkX][chunkY])
        {
            for (auto&& otherParticle : chunks_[otherSpecies][(chunkX + offsetX) % options_.getChunkCount()][(chunkY + offsetY) % options_.getChunkCount()]) {
                updateParticle(*particle, options_.getSpecies(species), otherParticle->getPosition(), otherSpecies);
            }
        }
    }
    void Simulation::updateParticle(Particle& particle, const ParticleSpecies& species, sf::Vector2f otherPos, size_t otherSpecies) {
        if (particle.getPosition() == otherPos)
            return;

        sf::Vector2f diff = otherPos - particle.getPosition();
        float halfWorldSize = options_.getWorldSize() / 2;
        if (diff.x < -halfWorldSize)
            diff.x += options_.getWorldSize();
        else if (diff.x > halfWorldSize)
            diff.x -= options_.getWorldSize();
        if (diff.y < -halfWorldSize)
            diff.y += options_.getWorldSize();
        else if (diff.y > halfWorldSize)
            diff.y -= options_.getWorldSize();

        float distanceSquared = diff.x * diff.x + diff.y * diff.y;

        if (distanceSquared >= species.attractionRange[otherSpecies] * species.attractionRange[otherSpecies])
            return;

        float distance = std::sqrtf(distanceSquared);
        float forceMagnitude = 0;
        float peak = (species.attractionRange[otherSpecies] + species.repulsionRange[otherSpecies]) / 2;
        float attractionChange = species.attraction[otherSpecies] / (peak - species.repulsionRange[otherSpecies]);

        if (distance < species.repulsionRange[otherSpecies]) {
            float overlap = 1 - distance / species.repulsionRange[otherSpecies];
            forceMagnitude = -options_.getRepulsion() * overlap * overlap;
        }
        else if (distance < peak) {
            forceMagnitude = (distance - species.repulsionRange[otherSpecies]) * attractionChange;
        }
        else {
            forceMagnitude = (species.attractionRange[otherSpecies] - distance) * attractionChange;
        }

        sf::Vector2f direction = diff / distance;
        particle.addForce(direction * forceMagnitude);
    }


    Simulation::Simulation(Options& options) :
        options_(options), simTime_(0), particles_(), chunks_() {}

    void Simulation::init()
    {
        updateParticleCounts();
    }

    void Simulation::tick() {
        simTime_ += options_.getTimeStep();
        updateParticleCounts();
        updateChunks();
        updateParticleForces();
        updateParticlePositions();
    }
}
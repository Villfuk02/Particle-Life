#include "ParticleSpecies.h"
namespace ParticleLife {
    ParticleSpecies::ParticleSpecies() :
        id(), color(), count(), repulsionRange(), attraction(), attractionRange(), chunkRange() {}

    ParticleSpecies::ParticleSpecies(size_t id_, const sf::Color& color_, size_t count_) :
        id(id_), color(color_), count(count_), repulsionRange(), attraction(), attractionRange(), chunkRange() {}
}
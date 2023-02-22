#include "Options.h"
namespace ParticleLife {
    const sf::Color* DEFAULT_COLORS[] = { &sf::Color::Green, &sf::Color::Red, &sf::Color::Blue, &sf::Color::Yellow, &sf::Color::White, &sf::Color::Magenta, &sf::Color::Cyan };
    const size_t DEFAULT_COLORS_AMT = sizeof(DEFAULT_COLORS) / sizeof(*DEFAULT_COLORS);
    constexpr int MAIN_COLOR_MIN_VALUE = 60;
    constexpr int MAX_COLOR_VALUE = 255;

    constexpr float DEFAULT_MAX_ATTRACTION_MAGNITUDE = 8;
    constexpr float DEFAULT_MAX_ATTRACTION_RANGE = 8;
    constexpr float DEFAULT_MIN_REPULSION_RANGE = 0.5;
    constexpr float DEFAULT_MAX_REPULSION_RANGE = 1.5;

    constexpr size_t DEFAULT_SPECIES_COUNT = 8;
    constexpr size_t DEFAULT_PARTICLE_COUNT = 120;

    Options::Options() :
        frameTime_(1 / 60.0f), timeStep_(1 / 60.0f), simSpeed_(1), realTimeStep_(timeStep_ / simSpeed_), worldSize_(100), friction_(0.25f), frictionMultiplierPerTick_(std::powf(friction_, timeStep_)),
        particleRadius_(0.3f), repulsion_(200), chunkCount_(16), chunkSize_(worldSize_ / chunkCount_), maxChunkRange_(), species_(), chunkPattern_(), random(time(nullptr)), paused(false), step(false)
    {
        for (size_t i = 0; i < DEFAULT_SPECIES_COUNT; i++)
        {
            addRandomSpecies(DEFAULT_PARTICLE_COUNT);
        }
        recalculateChunks();
    }

    void Options::setFPS(float fps) {
        frameTime_ = 1 / fps;
    }
    float Options::getFrameTime() const {
        return frameTime_;
    }
    void Options::setTimeStep(float timeStep) {
        timeStep_ = timeStep;
        realTimeStep_ = timeStep_ / simSpeed_;
    }
    float Options::getTimeStep() const {
        return timeStep_;
    }
    void Options::setSimulationSpeed(float simulationSpeed) {
        simSpeed_ = simulationSpeed;
        realTimeStep_ = timeStep_ / simSpeed_;
    }
    float Options::getSimulationSpeed() const {
        return simSpeed_;
    }
    float Options::getRealTimeStep() const {
        return realTimeStep_;
    }
    void Options::setWorldSize(float worldSize) {
        worldSize_ = worldSize;
        chunkSize_ = worldSize / chunkCount_;
        recalculateChunks();
    }
    float Options::getWorldSize() const {
        return worldSize_;
    }
    void Options::setFriction(float coefficient) {
        friction_ = coefficient;
        frictionMultiplierPerTick_ = std::powf(friction_, timeStep_);
    }
    float Options::getFriction() const {
        return friction_;
    }
    float Options::getFrictionMultiplierPerTick() const {
        return frictionMultiplierPerTick_;
    }
    void Options::setParticleRadius(float particleRadius) {
        particleRadius_ = particleRadius;
    }
    float Options::getParticleRadius() const {
        return particleRadius_;
    }
    void Options::setRepulsion(float repulsion) {
        repulsion_ = repulsion;
    }
    float Options::getRepulsion() const {
        return repulsion_;
    }
    void Options::setChunkCount(size_t count) {
        chunkCount_ = count;
        chunkSize_ = worldSize_ / count;
        recalculateChunks();
    }
    size_t Options::getChunkCount() const {
        return chunkCount_;
    }
    float Options::getChunkSize() const {
        return chunkSize_;
    }

    void Options::recalculateChunks() {
        float prevChunkRange = maxChunkRange_;
        for (auto&& s : species_) {
            for (auto&& r : s.attractionRange) {
                float chunkRange = r / chunkSize_;
                if (chunkRange > maxChunkRange_)
                    maxChunkRange_ = chunkRange;
            }
        }
        if (maxChunkRange_ > prevChunkRange)
            recalculateChunkPattern();
        recalculateChunkRanges();
    }
    void Options::recalculateChunkPattern() {
        chunkPattern_ = {};
        size_t ceilRange = std::ceilf(maxChunkRange_);
        for (unsigned int x = 1; x <= ceilRange; x++)
        {
            for (unsigned int y = 0; y <= ceilRange; y++)
            {
                chunkPattern_.emplace_back(x, y);
            }
        }
        std::sort(chunkPattern_.begin(), chunkPattern_.end(), chunkDistanceComparer());
        float maxRangeSquared = maxChunkRange_ * maxChunkRange_;
        chunkDistanceSquared distSq = {};
        size_t i = 0;
        for (auto&& e : chunkPattern_)
        {
            if (distSq(e) >= maxRangeSquared)
                break;
            i++;
        }
        chunkPattern_.resize(i);
    }
    void Options::recalculateChunkRanges() {
        chunkDistanceSquared distSq = {};
        std::vector<size_t> distsSq = {};
        for (auto&& e : chunkPattern_)
        {
            distsSq.push_back(distSq(e));
        }
        float chunkSqMultiplier = 1 / (chunkSize_ * chunkSize_);
        for (auto&& s : species_) {
            s.chunkRange.resize(s.attractionRange.size());
            for (size_t i = 0; i < s.attractionRange.size(); i++)
            {
                float rangeSq = s.attractionRange[i] * s.attractionRange[i] * chunkSqMultiplier;
                size_t r = 0;
                for (auto&& d : distsSq)
                {
                    if (d >= rangeSq)
                        break;
                    r++;
                }
                s.chunkRange[i] = r;
            }
        }
    }
    size_t Options::chunkDistanceSquared::operator()(sf::Vector2u chunkPos) {
        size_t x = chunkPos.x - 1;
        if (chunkPos.y == 0)
            return x * x;
        size_t y = chunkPos.y - 1;
        return x * x + y * y;
    }
    bool Options::chunkDistanceComparer::operator()(sf::Vector2u a, sf::Vector2u b) {
        chunkDistanceSquared c = {};
        return c(a) < c(b);
    }

    void Options::addRandomSpecies(size_t particleCount) {
        std::uniform_real_distribution<float> attractionDistribution(-DEFAULT_MAX_ATTRACTION_MAGNITUDE, DEFAULT_MAX_ATTRACTION_MAGNITUDE);
        std::uniform_real_distribution<float> repulsionRangeDistribution(DEFAULT_MIN_REPULSION_RANGE, DEFAULT_MAX_REPULSION_RANGE);
        std::uniform_real_distribution<float> attractionRangeDistribution(DEFAULT_MAX_REPULSION_RANGE, DEFAULT_MAX_ATTRACTION_RANGE);
        sf::Color color = species_.size() < DEFAULT_COLORS_AMT ? *DEFAULT_COLORS[species_.size()] : getRandomColor();
        ParticleSpecies s = ParticleSpecies(species_.size(), color, particleCount);
        for (size_t i = 0; i < species_.size() + 1; i++)
        {
            s.attraction.push_back(attractionDistribution(random));
            s.repulsionRange.push_back(repulsionRangeDistribution(random));
            s.attractionRange.push_back(attractionRangeDistribution(random));
        }
        for (auto&& sp : species_) {
            sp.attraction.push_back(attractionDistribution(random));
            sp.repulsionRange.push_back(repulsionRangeDistribution(random));
            sp.attractionRange.push_back(attractionRangeDistribution(random));
        }
        species_.push_back(s);
        recalculateChunks();
    }

    sf::Color Options::getRandomColor() {
        std::uniform_int_distribution<size_t> mainColorDist(MAIN_COLOR_MIN_VALUE, MAX_COLOR_VALUE);
        std::uniform_int_distribution<size_t> auxColorDist(0, MAX_COLOR_VALUE);
        std::uniform_int_distribution<size_t> selectorDist(0, 2);
        size_t selector = selectorDist(random);
        return sf::Color(
            selector == 0 ? mainColorDist(random) : auxColorDist(random),
            selector == 1 ? mainColorDist(random) : auxColorDist(random),
            selector == 2 ? mainColorDist(random) : auxColorDist(random)
        );
    }

    size_t Options::getSpeciesCount() const {
        return species_.size();
    }
    const ParticleSpecies& Options::getSpecies(size_t id) const {
        return species_[id];
    }
    void Options::setParticleCount(size_t id, size_t count) {
        species_[id].count = count;
    }
    void Options::setSpeciesColor(size_t id, sf::Color color) {
        species_[id].color = color;
    }
    void Options::setSpeciesAttraction(size_t id, size_t other, float attraction) {
        species_[id].attraction[other] = attraction;
    }
    void Options::setSpeciesAttractionRange(size_t id, size_t other, float range) {
        species_[id].attractionRange[other] = range;
        recalculateChunks();
    }
    void Options::setSpeciesRepulsionRange(size_t id, size_t other, float range) {
        species_[id].repulsionRange[other] = range;
        recalculateChunks();
    }
    const std::vector<sf::Vector2u>& Options::getChunkPattern() const
    {
        return chunkPattern_;
    }
}
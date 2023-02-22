#include "Command.h"
#include <iostream>
#include <format>
#include <iomanip>
#include <sstream>
namespace ParticleLife {
    constexpr float MAX_WORLD_SIZE = 1e9;
    constexpr size_t MAX_CHUNKS = 1000;
    constexpr size_t MIN_WORLD_SIZE_RATIO = 3;

    constexpr size_t TABLE_CELL_WIDTH = 7;
    constexpr size_t TABLE_CELL_SPACING = 3;
    constexpr size_t MIN_INTEGER_PART_WIDTH = 2;
    constexpr size_t SPECIES_ID_WIDTH = 2;
    constexpr size_t HEADER_SPACING_WIDTH = 5;

    bool FPSCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float fps;
        if (!parser_.parseNonNegativeFloat(getArguments()[0], args[1], fps))
            return false;
        options.setFPS(fps);
        return true;
    }

    void FPSCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Target FPS: " << 1 / options.getFrameTime() << std::endl;
    }

    void FPSCommand::printCommandDescription() const
    {
        std::cout << "Set target frames per second." << std::endl;
    }

    std::vector<std::string> FPSCommand::getArguments() const
    {
        return { "FPS" };
    }

    bool ColorCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t s, r, g, b;
        if (!parser_.parseSizeT(getArguments()[0], args[1], s, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseSizeT(getArguments()[1], args[2], r, 255))
            return false;
        if (!parser_.parseSizeT(getArguments()[2], args[3], g, 255))
            return false;
        if (!parser_.parseSizeT(getArguments()[3], args[4], b, 255))
            return false;
        options.setSpeciesColor(s, { (sf::Uint8)r,(sf::Uint8)g,(sf::Uint8)b });
        return true;
    }

    void ColorCommand::printCurrentSettings(const Options& options) const
    {
        for (size_t i = 0; i < options.getSpeciesCount(); i++)
        {
            sf::Color c = options.getSpecies(i).color;
            std::cout << "Color of species " << i << ": " << (size_t)c.r << " " << (size_t)c.g << " " << (size_t)c.b << std::endl;
        }
    }

    void ColorCommand::printCommandDescription() const
    {
        std::cout << "Set the display color of a particle species." << std::endl;
    }

    std::vector<std::string> ColorCommand::getArguments() const
    {
        return { "species", "R", "G", "B" };
    }

    bool HelpCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        for (auto&& e : commands_) {
            std::cout << " - " << e.first << ": ";
            e.second->printCommandDescription();
        }
        return true;
    }


    void HelpCommand::printCommandDescription() const
    {
        std::cout << "Prints this table." << std::endl;
    }


    bool ExitCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        exit(0);
    }


    void ExitCommand::printCommandDescription() const
    {
        std::cout << "Exit this application." << std::endl;
    }


    bool StepCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        options.paused = true;
        options.step = true;
        return true;
    }


    void StepCommand::printCommandDescription() const
    {
        std::cout << "Run a single tick of simulation." << std::endl;
    }


    bool PauseCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        options.paused = !options.paused;
        return true;
    }

    void PauseCommand::printCommandDescription() const
    {
        std::cout << "Pause or unpause the simulation." << std::endl;
    }


    bool RepulsionRangeCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t s, os;
        float r;
        if (!parser_.parseSizeT(getArguments()[0], args[1], s, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseSizeT(getArguments()[1], args[2], os, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseFloat(getArguments()[2], args[3], r, 0, options.getSpecies(s).attractionRange[os]))
            return false;
        options.setSpeciesRepulsionRange(s, os, r);
        return true;
    }

    void RepulsionRangeCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Repulsion range:" << std::endl;
        std::vector<std::vector<float>> values;
        for (size_t i = 0; i < options.getSpeciesCount(); i++)
        {
            values.push_back(options.getSpecies(i).repulsionRange);
        }
        Tabulate(std::move(values));
    }

    void RepulsionRangeCommand::printCommandDescription() const
    {
        std::cout << "Set the distance at which particles of given species start being repelled from particles of the other species. Must be less than the corresponding attraction range." << std::endl;
    }

    std::vector<std::string> RepulsionRangeCommand::getArguments() const
    {
        return { "species id", "other species id", "repulsion range" };
    }

    bool AttractionRangeCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t s, os;
        float r;
        if (!parser_.parseSizeT(getArguments()[0], args[1], s, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseSizeT(getArguments()[1], args[2], os, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseFloat(getArguments()[2], args[3], r, options.getSpecies(s).repulsionRange[os], options.getWorldSize() / MIN_WORLD_SIZE_RATIO))
            return false;
        options.setSpeciesAttractionRange(s, os, r);
        return true;
    }

    void AttractionRangeCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Attraction range:" << std::endl;
        std::vector<std::vector<float>> values;
        for (size_t i = 0; i < options.getSpeciesCount(); i++)
        {
            values.push_back(options.getSpecies(i).attractionRange);
        }
        Tabulate(std::move(values));
    }

    void AttractionRangeCommand::printCommandDescription() const
    {
        std::cout << "Set the distance at which particles of given species start being attracted to particles of the other species. Must be more than the corresponding repulsion range and less than third of world size." << std::endl;
    }

    std::vector<std::string> AttractionRangeCommand::getArguments() const
    {
        return { "species id", "other species id", "attraction range" };
    }

    bool AttractionCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t s, os;
        float r;
        if (!parser_.parseSizeT(getArguments()[0], args[1], s, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseSizeT(getArguments()[1], args[2], os, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseFloat(getArguments()[2], args[3], r))
            return false;
        options.setSpeciesAttraction(s, os, r);
        return true;
    }

    void AttractionCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Attraction strength:" << std::endl;
        std::vector<std::vector<float>> values;
        for (size_t i = 0; i < options.getSpeciesCount(); i++)
        {
            values.push_back(options.getSpecies(i).attraction);
        }
        Tabulate(std::move(values));
    }

    void AttractionCommand::printCommandDescription() const
    {
        std::cout << "Set the peak strength of attraction of particles of given species to particles of the other species. Can be negative, then the particles are repelled instead." << std::endl;
    }

    std::vector<std::string> AttractionCommand::getArguments() const
    {
        return { "species id", "other species id", "attraction strength" };
    }

    bool AddSpeciesCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t c;
        if (!parser_.parseSizeT(getArguments()[0], args[1], c))
            return false;
        options.addRandomSpecies(c);
        return true;
    }

    void AddSpeciesCommand::printCommandDescription() const
    {
        std::cout << "Add a new particle species with given particle count." << std::endl;
    }

    std::vector<std::string> AddSpeciesCommand::getArguments() const
    {
        return { "count" };
    }

    bool ChunkCountCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t c;
        if (!parser_.parseSizeT(getArguments()[0], args[1], c, MIN_WORLD_SIZE_RATIO, MAX_CHUNKS))
            return false;
        options.setChunkCount(c);
        return true;
    }

    void ChunkCountCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "The world is split into " << options.getChunkCount() << " by " << options.getChunkCount() << " chunks. Their size is " << options.getChunkSize() << "x" << options.getChunkSize() << "." << std::endl;
    }

    void ChunkCountCommand::printCommandDescription() const
    {
        std::cout << "The world will be split along each axis into a given amount of chunks. This setting won't affect the simulation, but will affect computation time." << std::endl;
    }

    std::vector<std::string> ChunkCountCommand::getArguments() const
    {
        return { "amount" };
    }

    bool RepulsionCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parsePositiveFloat(getArguments()[0], args[1], r))
            return false;
        options.setRepulsion(r);
        return true;
    }

    void RepulsionCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Peak repulsion strength: " << options.getRepulsion() << std::endl;
    }

    void RepulsionCommand::printCommandDescription() const
    {
        std::cout << "Set peak repulsion strength." << std::endl;
    }

    std::vector<std::string> RepulsionCommand::getArguments() const
    {
        return { "repulsion strength" };
    }

    bool ParticleRadiusCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parseNonNegativeFloat(getArguments()[0], args[1], r))
            return false;
        options.setParticleRadius(r);
        return true;
    }

    void ParticleRadiusCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Particle display radius: " << options.getParticleRadius() << std::endl;
    }

    void ParticleRadiusCommand::printCommandDescription() const
    {
        std::cout << "Set radius of the particles as displayed to the screen. If set to 0, rendering will be much faster and particles will be rendered as 1px points." << std::endl;
    }

    std::vector<std::string> ParticleRadiusCommand::getArguments() const
    {
        return { "display radius" };
    }

    bool FrictionCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parseFloat(getArguments()[0], args[1], r, 0, 1))
            return false;
        options.setFriction(1 - r);
        return true;
    }

    void FrictionCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Particles lose " << (1 - options.getFriction()) * 100 << "% of their velocity per second (" << (1 - options.getFrictionMultiplierPerTick()) * 100 << "% each tick)." << std::endl;
    }

    void FrictionCommand::printCommandDescription() const
    {
        std::cout << "Set how fast particles lose their momentum." << std::endl;
    }

    std::vector<std::string> FrictionCommand::getArguments() const
    {
        return { "friction" };
    }

    bool WorldSizeCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float maxRange = 0;
        for (size_t s = 0; s < options.getSpeciesCount(); s++) {
            for (auto&& r : options.getSpecies(s).attractionRange)
            {
                if (r > maxRange)
                    maxRange = r;
            }
        }
        float r;
        if (!parser_.parseFloat(getArguments()[0], args[1], r, maxRange * MIN_WORLD_SIZE_RATIO, MAX_WORLD_SIZE))
            return false;
        options.setWorldSize(r);
        return true;
    }

    void WorldSizeCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "The dimensions of the world are " << options.getWorldSize() << " by " << options.getWorldSize() << "." << std::endl;
    }

    void WorldSizeCommand::printCommandDescription() const
    {
        std::cout << "Set world size. It must be greater than three times the largest attraction range." << std::endl;
    }

    std::vector<std::string> WorldSizeCommand::getArguments() const
    {
        return { "size" };
    }

    bool SimSpeedCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parsePositiveFloat(getArguments()[0], args[1], r))
            return false;
        options.setSimulationSpeed(r);
        return true;
    }

    void SimSpeedCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Target simulation speed: " << options.getSimulationSpeed() << "x. Target ticks to simulate per second: " << 1 / options.getRealTimeStep() << "." << std::endl;
    }

    void SimSpeedCommand::printCommandDescription() const
    {
        std::cout << "Set simulation speed." << std::endl;
    }

    std::vector<std::string> SimSpeedCommand::getArguments() const
    {
        return { "speed multiplier" };
    }

    bool TimeStepCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parsePositiveFloat(getArguments()[0], args[1], r))
            return false;
        options.setTimeStep(r);
        return true;
    }

    void TimeStepCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Time step: " << options.getTimeStep() << " seconds. " << 1 / options.getTimeStep() << " ticks make 1 second of simulation." << std::endl;
    }

    void TimeStepCommand::printCommandDescription() const
    {
        std::cout << "Change simulation time step. Too long time steps make simulation unstable. Inverse of \"tps\"." << std::endl;
    }

    std::vector<std::string> TimeStepCommand::getArguments() const
    {
        return { "step in seconds" };
    }

    bool TPSCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        float r;
        if (!parser_.parsePositiveFloat(getArguments()[0], args[1], r))
            return false;
        options.setTimeStep(1 / r);
        return true;
    }

    void TPSCommand::printCurrentSettings(const Options& options) const
    {
        std::cout << "Time step: " << options.getTimeStep() << " seconds. " << 1 / options.getTimeStep() << " ticks make 1 second of simulation." << std::endl;
    }

    void TPSCommand::printCommandDescription() const
    {
        std::cout << "Change number of ticks per second of simulation. Too long time steps make simulation unstable. Inverse of \"ts\"." << std::endl;
    }

    std::vector<std::string> TPSCommand::getArguments() const
    {
        return { "TPS" };
    }

    bool ParticleCountCommand::run(Options& options, const std::vector<std::string>& args) const
    {
        size_t s, c;
        if (!parser_.parseSizeT(getArguments()[0], args[1], s, options.getSpeciesCount() - 1))
            return false;
        if (!parser_.parseSizeT(getArguments()[1], args[2], c))
            return false;
        options.setParticleCount(s, c);
        return true;
    }

    void ParticleCountCommand::printCurrentSettings(const Options& options) const
    {
        for (size_t i = 0; i < options.getSpeciesCount(); i++)
        {
            std::cout << "Particles of species " << i << ": " << options.getSpecies(i).count << std::endl;
        }
    }

    void ParticleCountCommand::printCommandDescription() const
    {
        std::cout << "Change number of particles of a given species." << std::endl;
    }

    std::vector<std::string> ParticleCountCommand::getArguments() const
    {
        return { "species id", "particle count" };
    }

    void Command::Tabulate(std::vector<std::vector<float>>&& values) const
    {
        std::ostringstream s;
        std::cout << std::setw(TABLE_CELL_WIDTH + HEADER_SPACING_WIDTH) << "other s.>";
        for (size_t j = 0; j < values.size(); j++)
        {
            std::cout << std::setw(SPECIES_ID_WIDTH) << j << std::setw(TABLE_CELL_WIDTH + TABLE_CELL_SPACING - SPECIES_ID_WIDTH) << "";
        }
        std::cout << std::endl << " v species" << std::endl;
        for (size_t i = 0; i < values.size(); i++)
        {
            std::cout << std::setw(SPECIES_ID_WIDTH) << i << std::setw(HEADER_SPACING_WIDTH) << "";
            for (size_t j = 0; j < values[i].size(); j++)
            {
                s = {};
                s << std::setprecision(TABLE_CELL_WIDTH - MIN_INTEGER_PART_WIDTH) << std::setw(TABLE_CELL_WIDTH) << values[i][j];
                std::string str = s.str();
                if (str.size() > TABLE_CELL_WIDTH) {
                    s = {};
                    s << std::setprecision(2 * TABLE_CELL_WIDTH - str.size() - MIN_INTEGER_PART_WIDTH) << std::setw(TABLE_CELL_WIDTH) << values[i][j];
                    str = s.str();
                }
                std::cout << str << std::setw(TABLE_CELL_SPACING) << "";
            }
            std::cout << std::endl;
        }
    }
}
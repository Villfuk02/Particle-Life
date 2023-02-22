#include "ValueParser.h"
#include <iostream>
namespace ParticleLife {
    bool ValueParser::parseSizeT(const std::string& str, size_t& out) const
    {
        size_t len;
        try {
            out = std::stoull(str, &len);
        }
        catch (...) {
            return false;
        }
        return len == str.size();
    }

    bool ValueParser::parseFloat(const std::string& str, float& out) const
    {
        size_t len;
        try {
            out = std::stof(str, &len);
        }
        catch (...) {
            return false;
        }
        return len == str.size() && std::isfinite(out);
    }

    bool ValueParser::parseSizeT(const std::string& argName, const std::string& str, size_t& out) const
    {
        bool success = parseSizeT(str, out);
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid positive integer." << std::endl;
        return success;
    }

    bool ValueParser::parseSizeT(const std::string& argName, const std::string& str, size_t& out, size_t max) const
    {
        bool success = parseSizeT(str, out) && out <= max;
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid positive integer up to " << max << "." << std::endl;
        return success;
    }

    bool ValueParser::parseSizeT(const std::string& argName, const std::string& str, size_t& out, size_t min, size_t max) const
    {
        bool success = parseSizeT(str, out) && out <= max && out >= min;
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid integer between " << min << " and " << max << "." << std::endl;
        return success;
    }

    bool ValueParser::parseFloat(const std::string& argName, const std::string& str, float& out) const
    {
        bool success = parseFloat(str, out);
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid decimal number." << std::endl;
        return success;
    }

    bool ValueParser::parseNonNegativeFloat(const std::string& argName, const std::string& str, float& out) const
    {
        bool success = parseFloat(str, out) && !signbit(out);
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid non-negative decimal number." << std::endl;
        return success;
    }

    bool ValueParser::parsePositiveFloat(const std::string& argName, const std::string& str, float& out) const
    {
        bool success = parseFloat(str, out) && out > 0;
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid positive decimal number." << std::endl;
        return success;
    }

    bool ValueParser::parseFloat(const std::string& argName, const std::string& str, float& out, float min, float max) const
    {
        bool success = parseFloat(str, out) && out < max&& out > min;
        if (!success)
            std::cout << ERROR_TAG << argName << " must be a valid decimal number greater than " << min << " and less than " << max << "." << std::endl;
        return success;
    }
}
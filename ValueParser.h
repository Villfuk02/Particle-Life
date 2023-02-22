#ifndef VALUE_PARSER_H_
#define VALUE_PARSER_H_
#include <string>
namespace ParticleLife {
    constexpr char ERROR_TAG[] = "[ERROR] ";

    class ValueParser {
    private:
        /// @brief convert string to size_t
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parseSizeT(const std::string& str, size_t& out) const;
        /// @brief convert string to float
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parseFloat(const std::string& str, float& out) const;
    public:
        /// @brief parse argument string to size_t and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parseSizeT(const std::string& argName, const std::string& str, size_t& out) const;
        /// @brief parse argument string to size_t less than or equal to max and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @param max maximal allowed value
        /// @return did parsing succeed
        bool parseSizeT(const std::string& argName, const std::string& str, size_t& out, size_t max) const;
        /// @brief parse argument string to size_t between min and max and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @param min minimal allowed value
        /// @param max maximal allowed value
        /// @return did parsing succeed
        bool parseSizeT(const std::string& argName, const std::string& str, size_t& out, size_t min, size_t max) const;
        /// @brief parse argument string to float and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parseFloat(const std::string& argName, const std::string& str, float& out) const;
        /// @brief parse argument string to non-negative float and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parseNonNegativeFloat(const std::string& argName, const std::string& str, float& out) const;
        /// @brief parse argument string to positive float and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @return did parsing succeed
        bool parsePositiveFloat(const std::string& argName, const std::string& str, float& out) const;
        /// @brief parse argument string to float greater than min and less than max and print user-readable error in case of failure
        /// @param argName user readable argument name
        /// @param str string to parse
        /// @param out variable to store result
        /// @param min minimal allowed value
        /// @param max maximal allowed value
        /// @return did parsing succeed
        bool parseFloat(const std::string& argName, const std::string& str, float& out, float min, float max) const;
    };
}
#endif

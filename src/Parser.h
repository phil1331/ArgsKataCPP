#pragma once

#include "ArgsMainHeader.h"
#include "ArgumentHandler.h"

namespace ph
{
    class Parser
    {
    public:

        // Decleration: Constructor Parser
        [[nodiscard]] Parser(std::initializer_list<const char*> pSchema, Arguments pRawArguments);

        // Decleration: Rule of 5

        // Decleration: Copy constructor
        [[nodiscard]] Parser(const Parser& pParser) noexcept;

        // Decleration: Move constructor
        [[nodiscard]] Parser(Parser&& pParser) noexcept;

        // Decleration: Copy assignment operator
        [[nodiscard]] constexpr Parser& operator=(const Parser& pParser) noexcept;

        // Decleration: Move assignment operator
        [[nodiscard]] constexpr Parser& operator=(Parser&& pParser) noexcept;

        // Decleration: Desctructor
        ~Parser();

    public:

        [[nodiscard]] inline constexpr auto GetString(std::uint8_t pFlag) const // Get a string flag
        {
            if(const auto* val = std::get_if<const char*>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with type String.", pFlag));
        }

        [[nodiscard]] inline constexpr auto GetBool(std::uint8_t pFlag) const // Get a bool flag
        {
            if(const auto* val = std::get_if<bool>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with type bool.", pFlag));
        }

        [[nodiscard]] inline constexpr auto GetInt(std::uint8_t pFlag) const // Get an integer flag
        {
            if(const auto* val = std::get_if<std::int64_t>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with type int.", pFlag));
        }

        [[nodiscard]] inline constexpr auto GetFP(std::uint8_t pFlag) const // Get a floating point flag
        {
            if(const auto* val = std::get_if<long double>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with floating point type.", pFlag));
        }

        [[nodiscard]] inline constexpr const std::vector<long double>& GetFPList(std::uint8_t pFlag) const // Get a floating point list flag
        {
            if(const auto* val = std::get_if<std::vector<long double>>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with a list with floating point type.", pFlag));
        }

        [[nodiscard]] inline constexpr const std::vector<std::int64_t>& GetIntList(std::uint8_t pFlag) const // Get an integer list flag
        {
            if(const auto* val = std::get_if<std::vector<std::int64_t>>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with a list with integer type.", pFlag));
        }

        [[nodiscard]] inline constexpr const std::vector<std::string>& GetStringList(std::uint8_t pFlag) const // Get a string list flag
        {
            if(const auto* val = std::get_if<std::vector<std::string>>(&m_Args.at(pFlag)))
            {
                return *val;
            }
            throw std::runtime_error(std::format("There is no flag {0} with a list with string type.", pFlag));
        }

    private:

        // Validate Arguments - this method is called in the constructor and calls InitDefaultSchemeValues - it parses the raw arguments into
        // a hash map
        void ValidateArguments(std::initializer_list<const char*> pSchema, Arguments& pRawArguments);

        // Init Default values in args hashmap
        void InitDefaultSchemeValues(std::initializer_list<const char*> pSchema) noexcept;
        
        // Validate an argument
        [[nodiscard]] bool ValidateArgument(Arg arg);

    private:

        // Validates if a c string pFlag is formatted as a valid flag
        [[nodiscard]] inline bool IsFlag(const char* pFlag) const noexcept
        {
            return (strlen(pFlag) == 2 && pFlag[0] == '-' && 
                ((pFlag[1] >= 'a' && pFlag[1] <= 'z') || (pFlag[1] >= 'A' && pFlag[1] <= 'Z'))); //Validates Flag format
        }

    private: // Valid types are just bool, std::uint64_t, const char* and long double, as well as a list of every type except bools
        std::unordered_map<std::uint8_t, std::variant<bool, std::int64_t, const char*, long double, 
                std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>> m_Args;
    };
}
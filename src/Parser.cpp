#include "Parser.h"

namespace ph
{
    // Default constructor Parser::Parser
    [[nodiscard]] Parser::Parser(std::initializer_list<const char*> pSchema, Arguments pRawArguments) 
    {
        ValidateArguments(pSchema, pRawArguments);
    }

    // Initialization: Rule of 5

    // Initialization: Copy constructor
    [[nodiscard]] Parser::Parser(const Parser& pParser) noexcept
    {
        m_Args = pParser.m_Args;
    }

    // Initialization: Move constructor
    [[nodiscard]] Parser::Parser(Parser&& pParser) noexcept
    {
        m_Args = std::move(pParser.m_Args);
    }

    // Initialization: Copy assignment operator
    [[nodiscard]] constexpr Parser& Parser::operator=(const Parser& pParser) noexcept
    {
        m_Args = pParser.m_Args;
        return *this;
    }

    // Initialization: Move assignment operator
    [[nodiscard]] constexpr Parser& Parser::operator=(Parser&& pParser) noexcept
    {
        m_Args = std::move(pParser.m_Args);
        return *this;
    }

    // Initialization: Destructor (defaulted)
    Parser::~Parser() = default;

    void Parser::ValidateArguments(std::initializer_list<const char*> pSchema, Arguments& pRawArguments)
    {
        InitDefaultSchemeValues(pSchema);

        for(int i = 1; i < pRawArguments.argc; i++) // Iterating over all raw arguments
        {
            if(IsFlag(pRawArguments.argv[i])) // Current argument is flag
            {
                if(i != pRawArguments.argc - 1 && !IsFlag(pRawArguments.argv[i + 1])) // No boolean argument
                {
                    if(!ValidateArgument(Arg{ .flag = pRawArguments.argv[i], .data = pRawArguments.argv[i + 1]}))
                    {
                        std::cerr << "Unknown argument: " << pRawArguments.argv[i] << " with data " << pRawArguments.argv[i + 1] << '\n';
                        std::cerr << "You might have to specify an other flag.\n";
                    }

                    i++;
                }
                else // boolean argument
                {
                    if(!ValidateArgument(Arg{ .flag = pRawArguments.argv[i], .data = nullptr}))
                    {
                        std::cerr << "Unknown boolean argument: " << pRawArguments.argv[i] << '\n';
                        std::cerr << "You might have to specify an other flag.\n";
                    }
                }
            }
            else
            {
                std::cout << "Unknown value with no assosciated key: " << pRawArguments.argv[i] << '\n';
                std::cerr << "A value always needs a flag as an affiliate. Did you forget the quotation marks?\n";
            }
        }
    }

    void Parser::InitDefaultSchemeValues(std::initializer_list<const char*> pSchema) noexcept
    {
        const auto regScheme = std::regex("[a-zA-Z](#|\\+|')\\.?"); // Scheme format
        const auto invalidIterator = std::sregex_iterator{};

        for(const auto& scheme : pSchema)
        {
            if(!strcmp(scheme, ""))
            {
                std::cerr << "Invalid scheme: empty scheme.\n";
                continue;
            }

            if(strlen(scheme) == 1 && ((scheme[0] <= 'z' && scheme[0] >= 'a') || (scheme[0] >= 'A' && scheme[0] <= 'Z')))
            {
                m_Args[scheme[0]] = false;
                continue;
            }
            else if(strlen(scheme) == 1)
            {
                continue;
            }

            const std::string schemeRef = scheme;
            std::string parsedCurScheme{};

            if(const auto iter = std::sregex_iterator(schemeRef.begin(), schemeRef.end(), regScheme); iter != invalidIterator)
            {
                parsedCurScheme = iter->str();
                if(parsedCurScheme.empty() || (parsedCurScheme.size() != 2 && parsedCurScheme.size() != 3))
                {
                    std::cerr << "Invalid scheme item: " << schemeRef << '\n';
                    continue;
                }
            }
            else
            {
                std::cerr << "Invalid scheme item: " << schemeRef << '\n';
                continue;
            }

            if(m_Args.contains(parsedCurScheme[0]))
            {
                std::cerr << "Invalid scheme item, already present one or more times in buffer: " << parsedCurScheme[0] << '\n';
                continue;
            }

            bool isList = false;

            if(parsedCurScheme.size() == 3 && parsedCurScheme[2] == '.')
            {
                isList = true;
            }
            else if(parsedCurScheme.size() == 3)
            {
                std::cerr << "Fatal: programming error found: invalid scheme: " << parsedCurScheme << '\n';
                continue;   
            }

            switch(parsedCurScheme[1])
            {
                case '\'':

                    m_Args[parsedCurScheme[0]] = (isList ? std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(std::vector<std::string>{}) 
                    : std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(""));

                    break;
                case '#':

                    m_Args[parsedCurScheme[0]] = (isList ? std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(std::vector<long double>{}) 
                    : std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(0.0));

                    break;
                case '+':

                    m_Args[parsedCurScheme[0]] = (isList ? std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(std::vector<std::int64_t>{}) 
                    : std::variant<bool, std::int64_t, const char*, long double, 
                        std::vector<std::int64_t>, std::vector<std::string>, std::vector<long double>>(0));

                    break;
                default:
                    std::cerr << "Invalid parsed scheme: " << parsedCurScheme << '\n';
            }
        }
    }

    [[nodiscard]] bool Parser::ValidateArgument(Arg arg)
    {
        if(arg.flag == nullptr || !IsFlag(arg.flag))
        {
            std::cerr << "Fatal: programming error in ValidateArgumentScheme method: arg does not have a valid flag.\n";
            throw std::runtime_error("Fatal: programming error in ValidateArgumentScheme method: arg does not have a valid flag.");
        }

        if(!m_Args.contains(arg.flag[1]))
        {
            return false;
        }

        if(arg.data == nullptr) // Current Argument is bool
        {
            if(const bool* ptrValue = std::get_if<bool>(&m_Args[arg.flag[1]]); !ptrValue)
            {
                std::cerr << "Wrong type specified on key: " << arg.flag << '\n';
                return true;
            }

            m_Args[arg.flag[1]] = true;
        }
        else // Current Argument is any other type
        {
            if(std::get_if<std::int64_t>(&m_Args[arg.flag[1]])) // Integer flag
            {
                try
                {
                    const std::int64_t value = std::stoll(arg.data);
                    m_Args[arg.flag[1]] = value; 
                }
                catch(...)
                {
                    std::cerr << "Data of key " << arg.flag << " has wrong data type: " << arg.data << '\n';
                }
            }
            else if (std::get_if<long double>(&m_Args[arg.flag[1]])) // Floating point flag
            {
                try
                {
                    const long double value = std::stold(arg.data);
                    m_Args[arg.flag[1]] = value; 
                }
                catch(...)
                {
                    std::cerr << "Data of key " << arg.flag << " has wrong data type: " << arg.data << '\n';
                }
            }
            else if (std::get_if<const char*>(&m_Args[arg.flag[1]])) // String flag
            {
                m_Args[arg.flag[1]] = arg.data;
            }
            else if (std::get_if<std::vector<std::string>>(&m_Args[arg.flag[1]])) // String List
            {
                const auto regParseChList = std::regex("[^,]+", std::regex::optimize);
                const auto invalidIterator = std::sregex_iterator{};

                const std::string dataCopy = arg.data;
                std::vector<std::string>& listRef = std::get<std::vector<std::string>>(m_Args[arg.flag[1]]);

                for(auto iter = std::sregex_iterator(dataCopy.begin(), dataCopy.end(), regParseChList); iter != invalidIterator; ++iter)
                {
                    listRef.push_back(iter->str());
                }

                if(listRef.empty())
                {
                    std::cerr << "Could not find any list items to add into list with flag " << arg.flag << " and data " << arg.data << '\n';
                }
            }
            else if (std::get_if<std::vector<std::int64_t>>(&m_Args[arg.flag[1]])) // Integer list
            {
                const auto regParseIntList = std::regex("-?[\\d]+", std::regex::optimize);
                const auto invalidIterator = std::sregex_iterator{};

                const std::string dataCopy = arg.data;
                std::vector<std::int64_t>& listRef = std::get<std::vector<std::int64_t>>(m_Args[arg.flag[1]]);

                for(auto iter = std::sregex_iterator(dataCopy.begin(), dataCopy.end(), regParseIntList); iter != invalidIterator; ++iter)
                {
                    if(!iter->str().empty())
                    {
                        try
                        {
                            const std::int64_t value = std::stoll(iter->str());
                            listRef.push_back(value);
                        }
                        catch(...)
                        {
                            std::cerr << "Data of key " << arg.flag << " with type list has wrong data types: " << arg.data << '\n';
                        }
                    } 
                }

                if(listRef.empty())
                {
                    std::cerr << "Could not find any list items to add into list with flag " << arg.flag << " and data " << arg.data << '\n';
                }
            }
            else if (std::get_if<std::vector<long double>>(&m_Args[arg.flag[1]])) // Floating point list
            {
                const auto regParseFPList = std::regex("-?[\\d]+\\.[\\d]+", std::regex::optimize);
                const auto invalidIterator = std::sregex_iterator{};

                const std::string dataCopy = arg.data;
                std::vector<long double>& listRef = std::get<std::vector<long double>>(m_Args[arg.flag[1]]);

                for(auto iter = std::sregex_iterator(dataCopy.begin(), dataCopy.end(), regParseFPList); iter != invalidIterator; ++iter)
                {
                    if(!iter->str().empty())
                    {
                        try
                        {
                            const long double value = std::stold(iter->str());
                            listRef.push_back(value);
                        }
                        catch(...)
                        {
                            std::cerr << "Data of key " << arg.flag << " with type list has wrong data types: " << arg.data << '\n';
                        }
                    }
                }

                if(listRef.empty())
                {
                    std::cerr << "Could not find any list items to add into list with flag " << arg.flag << " and data " << arg.data << '\n';
                }
            }
            else
            {
                std::cerr << "Fatal: invalid type at buffer.\n";
                throw std::runtime_error("Fatal: invalid type at buffer.");
            }
        }
        return true;
    }
}
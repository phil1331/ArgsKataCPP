#pragma once

namespace ph
{
    // Wrapper struct for raw arguments
    struct Arguments
    {
        int argc;
        const char** argv;
    };

    // Arg struct to represent an argument with a flag and user-specified data. The data may be nullptr in case of a bool flag.
    struct Arg
    {
        const char* flag;
        const char* data;
    };
}
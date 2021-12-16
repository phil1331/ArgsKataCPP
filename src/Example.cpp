// Copyright (C) 2021 phil1331
// C++20 implementation of Args Kata, made by phil1331 :)

/**
 * Schema format:
 * 
 * d' string
 * d bool
 * d# floating point
 * d+ integer
 * d+. list of integers
 * d#. list of floating points
 * d'. list of strings
 */

#include "Parser.h"

int main(int argc, const char* argv[])
{
    const auto parser = ph::Parser({"l#.", "b'.", "A", "i+", "I+", "c'"}, ph::Arguments{.argc = argc, .argv = argv});

    for(const long double& item : parser.GetFPList('l'))
    {
        std::cout << item << '\n';
    }

    std::cout << "Boolean flag \"A\" is specified: " << std::boolalpha << parser.GetBool('A') << '\n';
    std::cout << "Two integers i and I summed up: " << (parser.GetInt('i') + parser.GetInt('I')) << '\n';
    std::cout << "String with flag 'c' is: " << parser.GetString('c') << '\n';

    const auto& fpList = parser.GetFPList('l');
    std::for_each(fpList.begin(), fpList.end(), [](const auto& pItem){ std::cout << pItem << '\n'; });

    puts("");

    const auto& stringList = parser.GetStringList('b');
    std::for_each(stringList.begin(), stringList.end(), [](const auto& pItem){std::cout << pItem << '\n'; });
}
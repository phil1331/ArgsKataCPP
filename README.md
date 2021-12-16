# **[Args Kata](https://codingdojo.org/kata/Args/)**
## C++20 implementation of Args Kata, made by phil1331.
## **Problem Description**

```
Most of us have had to parse command-line arguments from time to time. If we don’t have a convenient utility, then we simply walk the array of strings that is passed into the main function. There are several good utilities available from various sources, but they probably don’t do exactly what we want. So let’s write another one!

The arguments passed to the program consist of flags and values. Flags should be one character, preceded by a minus sign. Each flag should have zero, or one value associated with it.

You should write a parser for this kind of arguments. This parser takes a schema detailing what arguments the program expects. The schema specifies the number and types of flags and values the program expects.

Once the schema has been specified, the program should pass the actual argument list to the args parser. It will verify that the arguments match the schema. The program can then ask the args parser for each of the values, using the names of the flags. The values are returned with the correct types, as specified in the schema.

For example if the program is to be called with these arguments:

-l -p 8080 -d /usr/logs

this indicates a schema with 3 flags: l, p, d. The “l” (logging) flag has no values associated with it, it is a boolean flag, True if present, False if not. the “p” (port) flag has an integer value, and the “d” (directory) flag has a string value.

If a flag mentioned in the schema is missing in the arguments, a suitable default value should be returned. For example “False” for a boolean, 0 for a number, and “” for a string. If the arguments given do not match the schema, it is important that a good error message is given, explaining exactly what is wrong.

If you are feeling ambitious, extend your code to support lists eg

-g this,is,a,list -d 1,2,-3,5

So the “g” flag indicates a list of strings, [“this”, “is”, “a”, “list”] and the “d” flag indicates a list of integers, [1, 2, -3, 5].

Make sure your code is extensible, in that it is straightforward and obvious how to add new types of values.
```

### The parser is able to accept lists and any floating point, integer, string or bool data type.
### Following schemes are valid (x is a flag name):
```
x'      =>      String
x       =>      bool
x#      =>      floating point
x+      =>      integer
x+.     =>      list of integers
x#.     =>      list of floating points
x'.    =>      list of strings
```
### To use the parser, first you have make a cpp file and include Parser.h.
```cpp
#include "Parser.h"
```
### To initialize a parser declare it with a scheme and arguments passed in, for example like this:
```cpp
// This initializes a parser that expects a list of floating points and a list of strings in the passed in Arguments ph::Arguments
const auto parser = ph::Parser({"l#.", "b'."}, ph::Arguments{.argc = argc, .argv = argv});
```
### To access the different flags in the parser you can use the "Get" methods of the ph::Parser.
```cpp
// Access the floating point list and print every item.
for(const long double& item : parser.GetFPList('l'))
{
    std::cout << item << '\n';
}

puts("");

// Access the string list and print every item.
for(const auto& str : parser.GetStringList('b'))
{
    std::cout << str << '\n';
}
```
### Or access them like this:
```cpp
// Access the floating point list and print every item.
const auto& fpList = parser.GetFPList('l');
std::for_each(fpList.begin(), fpList.end(), [](const auto& pItem){ std::cout << pItem << '\n'; });

puts("");

// Access the string list and print every item.
const auto& stringList = parser.GetStringList('b');
std::for_each(stringList.begin(), stringList.end(), [](const auto& pItem){std::cout << pItem << '\n'; });
```
### These are all methods to access items:
```
GetString : Default value "" (const char*, c string)
GetBool : Default value false (bool)
GetInt : Default value 0 (std::int64_t, 64 bit signed integer)
GetFP : Default value 0.0 (long double, floating point data type with widest range in c++)
GetFPList : Default value std::vector<long double>{} (empty list fp data type with widest range)
GetIntList : Default value std::vector<std::int64_t>{}(empty list, 64 bit signed integer)
GetStringList : Default value std::vector<std::string>{}(empty list, standard string)
```
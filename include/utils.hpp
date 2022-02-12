/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#ifndef utils_hpp_
#define utils_hpp_

#include <string>
#include <sstream>
#include <vector>

template <typename Type>
inline Type toNum(std::string number)
{
    static_assert(std::is_arithmetic<Type>::value, "Not an arithmetic type");
    Type value;
    std::istringstream stream(number);

    stream >> value;
    if (stream.fail() || !stream.eof())
        throw (std::runtime_error("'" + number + "' is not valid"));
    return (value);
}

template <typename Type>
inline std::vector<Type> toNum(std::vector<std::string> numbers)
{
    static_assert(std::is_arithmetic<Type>::value, "Not an arithmetic type");
    std::vector<Type> values;

    for (std::string s : numbers) {
        values.push_back(toNum<Type>(s));
    }
    return (values);
}

#endif /* utils_hpp_ */
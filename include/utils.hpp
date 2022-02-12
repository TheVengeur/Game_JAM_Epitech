/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#ifndef utils_hpp_
#define utils_hpp_

#include <filesystem>
#include <string>
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

static inline bool endsWith(const std::string &string, const std::string &suffix)
{
    return (string.find_last_of(suffix) == string.size() - 1);
}

inline std::vector<std::string> getFiles(std::string dirPath)
{
    std::vector<std::string> files;
    std::filesystem::path p(dirPath);

    for (const std::filesystem::directory_entry &dir_entry : std::filesystem::directory_iterator(p)) {
        if (dir_entry.is_regular_file() && endsWith(dir_entry.path(), ".qPUG"))
            files.push_back(dir_entry.path());
    }
    return (files);
}

#endif /* utils_hpp_ */
/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include <iostream>
#include <fstream>

#include "utils.hpp"
#include "Session.hpp"

int main(int argc, char *argv[]) try
{
    if (argc > 2)
        throw (std::runtime_error("Invalid argument count\nUsage: " + std::string(argv[0]) + " [seed]"));
    else if (argc == 2)
        srandom(toNum<unsigned int>(argv[1]));
    else
        srandom(time(nullptr));

    std::ifstream inputFile;

    for (const std::string &filePath : getFiles("questions")) {
        qPUG::Session session(filePath);
        session.printAllQuestions();
    }
    return (0);
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (84);
}
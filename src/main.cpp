/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include <iostream>

#include "utils.hpp"
#include "Pascal.hpp"

int main(int argc, char *argv[]) try
{
    if (argc > 2)
        throw (std::runtime_error("Invalid argument count\nUsage: " + std::string(argv[0]) + " [seed]"));
    else if (argc == 2)
        srandom(toNum<unsigned int>(argv[1]));
    else
        srandom(time(nullptr));

    qPUG::Pascal game;
    game.start("questions");

    return (0);
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (84);
}
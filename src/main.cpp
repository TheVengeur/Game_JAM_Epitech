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
        session.loadQuestions();
        for (std::size_t idx = 0; idx < qPUG::Session::questionsPerSession; idx++) {
            std::cout << session[idx].question;
            for (std::size_t jdx = 0; jdx < session[idx].propositions.size(); jdx++) {
                if (jdx == session[idx].answer) {
                    std::cout << " \033[32m" << session[idx].propositions[jdx] << "\033[0m";
                } else
                    std::cout << " " << session[idx].propositions[jdx];
            }
            std::cout << std::endl;
        }
    }
    return (0);
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (84);
}
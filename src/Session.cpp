/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include "Session.hpp"
#include "utils.hpp"
#include <iostream>

namespace qPUG
{

static inline std::vector<std::string> split(std::string s, const std::string &delimiter = "\n")
{
    std::vector<std::string> res;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return (res);
}

Session::Session(const std::string &fPath) :
    _fPath(fPath), _totalQuestions(this->fileChecker())
{
    if (this->_totalQuestions == Session::fileError)
        throw (std::runtime_error(fPath + " isn't a valid qPUG file: " + this->_error));
    this->loadSessionData();
}



void Session::printAllQuestions(void)
{
    std::string buff;
    std::vector<std::string> tmp;

    this->_fStream.seekg(this->_questionsStart);
    while (!this->_fStream.eof()) {
        std::getline(this->_fStream, buff);
        tmp = split(buff, "\a");
        std::cout << tmp[0];
        for (std::size_t idx = 2; idx < tmp.size(); idx++) {
            if (idx - 2 == static_cast<std::size_t>(*reinterpret_cast<unsigned int *>(&tmp[1][0]))) {
                std::cout << " \033[32m" << tmp[idx] << "\033[0m";
            } else
                std::cout << " " << tmp[idx];
        }
        std::cout << std::endl;
    }
}



std::size_t Session::fileChecker(void)
{
    std::string buff;
    std::vector<std::string> tmp;
    std::size_t questions = 0;

    this->_fStream.open(this->_fPath);
    if (!this->_fStream.is_open())
        throw (std::runtime_error("Could not open " + this->_fPath));
    if (this->_fStream.eof()) {
        this->_error = "file is empty";
        return (Session::fileError);
    }
    std::getline(this->_fStream, buff);
    if (this->_fStream.eof()) {
        this->_error = "file has an incomplete header";
        return (Session::fileError);
    }
    tmp = split(buff, "\a");
    if (tmp.size() != 3 || tmp[0] != "qPUG") {
        this->_error = "file has a malformated header";
        return (Session::fileError);
    }
    std::getline(this->_fStream, buff);
    if (buff.find('\a') != std::string::npos) {
        this->_error = "file has an incomplete header";
        return (Session::fileError);
    }
    while (!this->_fStream.eof()) {
        std::getline(this->_fStream, buff);
        if (buff.length() == 0 || buff[0] == '#')
            continue;
        tmp = split(buff, "\a");
        if (tmp.size() < 4) {
            this->_error = "'" + tmp[0] + "' isn't a valid question";
            return (Session::fileError);
        }
        try {
            if (tmp[1].length() != 4 || static_cast<std::size_t>(*reinterpret_cast<unsigned int *>(&tmp[1][0])) >= tmp.size() - 2) {
                this->_error = "'" + tmp[0] + "' doesn't have a valid answer";
                return (Session::fileError);
            }
        } catch (const std::runtime_error &e) {
            this->_error = "'" + tmp[0] + "' doesn't have a valid answer";
            return (Session::fileError);
        }
        questions++;
    }
    if (questions >= Session::questionsPerSession)
        return (questions);
    this->_error = "file has less than " + std::to_string(Session::questionsPerSession) + " questions";
    return (Session::fileError);
}

void Session::loadSessionData(void)
{
    std::string buff;
    std::vector<std::string> tmp;

    this->_fStream.clear();
    this->_fStream.seekg(0);
    if (this->_fStream.fail())
        throw (std::runtime_error("How fuck"));
    std::getline(this->_fStream, buff);
    tmp = split(buff, "\a");
    this->_font = tmp[1];
    this->_language = tmp[2];
    std::getline(this->_fStream, this->_comment);
    this->_questionsStart = this->_fStream.tellg();
}

void Session::loadQuestions(void)
{
    std::array<std::size_t, Session::questionsPerSession> qIndexes;
    this->_fStream.seekg(this->_questionsStart);
}

}; /* namespace qPUG */
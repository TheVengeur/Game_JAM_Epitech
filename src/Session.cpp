/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include "Session.hpp"
#include "utils.hpp"
#include <iostream>
#include <algorithm>

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

    this->seekg(this->_questionsStart);
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



void Session::loadQuestions(void)
{
    std::array<std::size_t, Session::questionsPerSession> qIndexes;
    std::array<std::size_t, Session::questionsPerSession> qSortedIndexes;
    std::string buff;
    std::vector<std::string> tmp;
    bool valid = true;
    std::size_t qIndex = 0;
    std::size_t question = 0;
    Session::Question qBuff;

    for (std::size_t idx = 0; idx < Session::questionsPerSession; idx++) {
        qIndexes[idx] = random() % this->_totalQuestions;
        do {
            valid = true;
            for (std::size_t jdx = 0; jdx < idx; jdx++) {
                if (qIndexes[idx] == qIndexes[jdx]) {
                    valid = false;
                    qIndexes[idx] = (qIndexes[idx] + 1) % this->_totalQuestions;
                }
            }
        } while (!valid);
    }
    qSortedIndexes = qIndexes;
    std::sort(qSortedIndexes.begin(), qSortedIndexes.end());

    this->seekg(this->_questionsStart);
    while (question <= qSortedIndexes[Session::questionsPerSession - 1]) {
        std::getline(this->_fStream, buff);
        if (question == qSortedIndexes[qIndex]) {
            tmp = split(buff, "\a");
            qBuff.propositions.clear();
            qBuff.question = tmp[0];
            qBuff.answer = static_cast<std::size_t>(*reinterpret_cast<unsigned int *>(&tmp[1][0]));
            for (std::size_t idx = 2; idx < tmp.size(); idx++)
                qBuff.propositions.push_back(tmp[idx]);
            for (std::size_t idx = 0; idx < Session::questionsPerSession; idx++) {
                if (qSortedIndexes[qIndex] == qIndexes[idx]) {
                    this->_questions[idx] = qBuff;
                    break;
                }
            }
            qIndex++;
        }
        question++;
    }
}

void Session::cleanQuestions(void)
{
    for (Session::Question &q : this->_questions) {
        q.question.clear();
        q.answer = 0;
        q.propositions.clear();
    }
}



std::string Session::getPath(void)
{
    return (this->_fPath);
}

std::string Session::getFont(void)
{
    return (this->_font);
}

std::string Session::getLanguage(void)
{
    return (this->_language);
}

std::string Session::getComment(void)
{
    return (this->_comment);
}



Session::Question Session::operator[](std::size_t idx)
{
    return (this->_questions[idx]);
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

    this->seekg(0);
    std::getline(this->_fStream, buff);
    tmp = split(buff, "\a");
    this->_font = tmp[1];
    this->_language = tmp[2];
    std::getline(this->_fStream, this->_comment);
    this->_questionsStart = this->_fStream.tellg();
}

void Session::seekg(const std::streampos &n)
{
    this->_fStream.clear();
    this->_fStream.seekg(n);
}

}; /* namespace qPUG */
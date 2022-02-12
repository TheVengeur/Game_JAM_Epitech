/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#ifndef Session_hpp_
#define Session_hpp_

#include <string>
#include <fstream>
#include <vector>
#include <array>

namespace qPUG
{

class Session
{
    public:
        static constexpr std::size_t fileError = static_cast<std::size_t>(-1);
        static constexpr std::size_t questionsPerSession = 10ul;

        struct Question
        {
            std::string                 question;
            std::vector<std::string>    propositions;
            std::size_t                 answer;
        };

        Session(const std::string &fPath);
        ~Session() = default;

        void printAllQuestions(void);
        void loadQuestions(void);
        void cleanQuestions(void);

        std::string getPath(void);
        std::string getFont(void);
        std::string getLanguage(void);
        std::string getComment(void);

        Session::Question operator[](std::size_t idx);

    private:
        std::size_t fileChecker(void);
        void loadSessionData(void);
        void seekg(const std::streampos &n);

        std::string                                                 _error;
        std::string                                                 _fPath;
        std::string                                                 _font;
        std::string                                                 _language;
        std::string                                                 _comment;
        std::streampos                                              _questionsStart;
        std::ifstream                                               _fStream;
        std::size_t                                                 _totalQuestions;
        std::array<Session::Question, Session::questionsPerSession> _questions;
};

}; /* namespace qPUG */


#endif /* Session_hpp_ */
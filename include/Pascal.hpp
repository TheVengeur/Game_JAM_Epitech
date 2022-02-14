/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#ifndef Pascal_hpp_
#define Pascal_hpp_

#include "Session.hpp"

#include <SFML/Graphics.hpp>

namespace qPUG
{

class Pascal
{
    public:
        static const std::string &dFont;
        static const std::string &dRes;
        static const std::string &dPasc;
        static const std::string &dBg;
        static const std::string &dQBox;
        static const std::string &dABox;

        static constexpr unsigned int scrWidth = 800;
        static constexpr unsigned int scrHeight = 600;

        enum State
        {
            Menu,
            Playing,
            Score
        };

        Pascal(void);
        ~Pascal() = default;

        void start(const std::string &dirPath);

    private:
        void getFiles(const std::string &dirPath);

        void hEventMenu(sf::Event &event, std::size_t &idx, sf::Text &text);
        void hEventPlaying(sf::Event &event, std::size_t &idx, bool &validated, const std::vector<std::string> &propositions);
        void hEventScore(sf::Event &event, std::size_t &idx);

        std::vector<std::unique_ptr<qPUG::Session>> _files;
        State                                       _state;
        std::size_t                                 _curQ;
        sf::Font                                    _qFont;
        sf::Font                                    _mFont;
};

}; /* namespace qPUG */

#endif /* Pascal_hpp_ */
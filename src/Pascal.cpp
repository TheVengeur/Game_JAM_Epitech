/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include "Pascal.hpp"

#include <filesystem>
#include <iostream>

namespace qPUG
{

static inline bool endsWith(const std::string &string, const std::string &suffix)
{
    return (string.find_last_of(suffix) == string.size() - 1);
}

static inline std::string mkPath(const std::string &dir, const std::string &file)
{
    return (dir + "/" + file);
}



const std::string &Pascal::dFont = "arial.ttf";
const std::string &Pascal::dRes = "ressources";



Pascal::Pascal(void) :
    _state(Pascal::State::Menu), _curQ(0)
{
    if (!this->_mFont.loadFromFile(mkPath(Pascal::dRes, Pascal::dFont)))
        throw (std::runtime_error("Couldn't load font " + mkPath(Pascal::dRes, Pascal::dFont)));
}



void Pascal::start(const std::string &dirPath)
{
    this->getFiles(dirPath);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Question pour un Gorille");

    sf::Text text;
    text.setFont(this->_mFont);
    std::size_t idx = 0;
    std::size_t answer = 0;
    bool validated = false;
    std::size_t score = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (this->_state == Pascal::State::Menu)
                this->hEventMenu(event, idx, text);
            else if (this->_state == Pascal::State::Playing)
                this->hEventPlaying(event, answer, validated, (*this->_files[idx])[this->_curQ].propositions);
            else if (this->_state == Pascal::State::Score)
                this->hEventScore(event, idx);
        }
        if (this->_curQ >= Session::questionsPerSession) {
            this->_state = Pascal::State::Score;
            text.setFont(this->_mFont);
            this->_curQ = 0;
        }
        if (this->_state == Pascal::State::Menu) {
            text.setPosition(0, 0);
            text.setString(this->_files[idx]->getPath());
            window.draw(text);
            text.setPosition(0, text.getPosition().y + text.getLocalBounds().top + text.getLocalBounds().height);
            text.setString(this->_files[idx]->getLanguage());
            window.draw(text);
            text.setPosition(0, text.getPosition().y + text.getLocalBounds().top + text.getLocalBounds().height);
            text.setString(this->_files[idx]->getComment());
            window.draw(text);
            validated = false;
        } else if (this->_state == Pascal::State::Playing) {
            text.setPosition(0, 0);
            text.setString((*this->_files[idx])[this->_curQ].question);
            window.draw(text);
            for (std::size_t jdx = 0; jdx < (*this->_files[idx])[this->_curQ].propositions.size(); jdx++) {
                text.setPosition(0, text.getPosition().y + text.getLocalBounds().top + text.getLocalBounds().height);
                text.setString((*this->_files[idx])[this->_curQ].propositions[jdx]);
                if (answer == jdx)
                    text.setFillColor(sf::Color::Green);
                window.draw(text);
                text.setFillColor(sf::Color::White);
            }
            if (validated) {
                if (answer == (*this->_files[idx])[this->_curQ].answer)
                    score++;
                this->_curQ++;
                validated = false;
            }
        } else if (this->_state == Pascal::State::Score) {
            text.setPosition(0, 0);
            text.setString(std::to_string(score));
            window.draw(text);
        }
        window.display();
        window.clear();
    }
}



void Pascal::getFiles(const std::string &dirPath)
{
    std::filesystem::path p(dirPath);

    for (const std::filesystem::directory_entry &dir_entry : std::filesystem::directory_iterator(p)) {
        if (dir_entry.is_regular_file() && endsWith(dir_entry.path(), ".qPUG"))
            this->_files.push_back(std::make_unique<qPUG::Session>(dir_entry.path()));
    }
}



void Pascal::hEventMenu(sf::Event &event, std::size_t &idx, sf::Text &text)
{
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            idx = (idx + 1) % this->_files.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            idx = (idx + this->_files.size() - 1) % this->_files.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            this->_state = Pascal::State::Playing;
            this->_files[idx]->loadQuestions();
            this->_qFont.loadFromFile(mkPath(Pascal::dRes, this->_files[idx]->getFont()));
            text.setFont(this->_qFont);
        }
    }
}



void Pascal::hEventPlaying(sf::Event &event, std::size_t &answer, bool &validated, const std::vector<std::string> &propositions)
{
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            answer = (answer + propositions.size() - 1) % propositions.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            answer = (answer + 1) % propositions.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            validated = true;
    }
}



void Pascal::hEventScore(sf::Event &event, std::size_t &idx)
{
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            this->_files[idx]->cleanQuestions();
            this->_state = Pascal::State::Menu;
        }
    }
}

} /* namespace qPUG */
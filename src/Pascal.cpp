/*
** EPITECH JAM PROJECT
** -------------------
**   Chrysalde CARON
*/

#include "Pascal.hpp"

#include <filesystem>
#include <iostream>
#include <locale>
#include <codecvt>

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

static inline std::wstring toWString(const std::string &src)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return (converter.from_bytes(src));
}



const std::string &Pascal::dFont = "arial.ttf";
const std::string &Pascal::dRes = "ressources";
const std::string &Pascal::dPasc = "Pascal.png";
const std::string &Pascal::dBg = "Background_Jam.png";
const std::string &Pascal::dQBox = "True_Question_box.png";
const std::string &Pascal::dABox = "Reponse_box.png";



Pascal::Pascal(void) :
    _state(Pascal::State::Menu), _curQ(0)
{
    if (!this->_mFont.loadFromFile(mkPath(Pascal::dRes, Pascal::dFont)))
        throw (std::runtime_error("Couldn't load font " + mkPath(Pascal::dRes, Pascal::dFont)));
}



void justifyText(sf::Text &text, const sf::IntRect &bBox)
{
    text.setPosition(bBox.left, bBox.top);
    if (text.getLocalBounds().left + text.getLocalBounds().width < bBox.width &&
        text.getLocalBounds().top + text.getLocalBounds().height < bBox.height)
        return;
    if (text.getLocalBounds().left + text.getLocalBounds().width >=  bBox.width) {
        for (std::size_t idx = 0; idx < text.getString().getSize(); idx++) {
            if (text.findCharacterPos(idx).x + text.getFont()->getGlyph(text.getString()[idx], text.getCharacterSize(), false).bounds.width >= bBox.left + bBox.width) {
                sf::String str = text.getString();
                str[str.toWideString().find_last_of(' ', idx)] = '\n';
                text.setString(str);
            }
        }
    }
    if (text.getLocalBounds().top + text.getLocalBounds().height >= bBox.height && text.getCharacterSize() > 5) {
        sf::String str = text.getString();
        for (sf::Uint32 &c : str) {
            if (c == '\n')
                c = ' ';
        }
        text.setString(str);
        text.setCharacterSize(text.getCharacterSize() - 5);
        justifyText(text, bBox);
    }
}

static constexpr int questionOffset = 17;
static constexpr int answerOffset = 8;
static const std::array<std::string, 11> fMessages = {
    "Il arrive souvent de ne rien obtenir parce que l'on ne tente rien.",
    "Le commencement est beaucoup plus que la moitié de l'objectif.",
    "Il est dur d'échouer ; mais il est pire de n'avoir jamais tenté de réussir.",
    "Celui qui ne progresse pas chaque jour, recule chaque jour.",
    "Celui qui n'a pas d'objectifs ne risque pas de les atteindre",
    "Avoir un but trace la voie.",
    "Le sentier est unique pour tous, les moyens d'atteindre le but varient avec le voyageur.",
    "L'obstination est le chemin de la réussite.",
    "Pour atteindre l'objectif final, je me concentre d'abord sur la préparation.",
    "Accepte les défis afin que tu puisses sentir l'euphorie de la victoire.",
    "La plus grande victoire, c'est la victoire sur soi."
};

void Pascal::start(const std::string &dirPath)
{
    this->getFiles(dirPath);
    sf::RenderWindow window(sf::VideoMode(Pascal::scrWidth, Pascal::scrHeight), "Question pour un Gorille", sf::Style::Close);

    sf::Text text;
    text.setFont(this->_mFont);
    std::size_t idx = 0;
    std::size_t answer = 0;
    bool validated = false;
    std::size_t score = 0;

    sf::Texture texbg;
    if (!texbg.loadFromFile(mkPath(Pascal::dRes, Pascal::dBg)))
        throw (std::runtime_error("Couldn't load texture " + mkPath(Pascal::dRes, Pascal::dBg)));
    sf::Texture texpascal;
    if (!texpascal.loadFromFile(mkPath(Pascal::dRes, Pascal::dPasc)))
        throw (std::runtime_error("Couldn't load texture " + mkPath(Pascal::dRes, Pascal::dPasc)));
    sf::Texture texqestion;
    if (!texqestion.loadFromFile(mkPath(Pascal::dRes, Pascal::dQBox)))
        throw (std::runtime_error("Couldn't load texture " + mkPath(Pascal::dRes, Pascal::dQBox)));
    sf::Texture texanswer;
    if (!texanswer.loadFromFile(mkPath(Pascal::dRes, Pascal::dABox)))
        throw (std::runtime_error("Couldn't load texture " + mkPath(Pascal::dRes, Pascal::dABox)));

    sf::Sprite bg;
    bg.setTexture(texbg);
    sf::Sprite pascal;
    pascal.setTexture(texpascal);
    pascal.setTextureRect({0, 0, static_cast<int>(texpascal.getSize().x) / 3, static_cast<int>(texpascal.getSize().y)});
    pascal.setPosition(-pascal.getTextureRect().width / 6, Pascal::scrHeight - pascal.getTextureRect().height);
    sf::Sprite qBox;
    qBox.setTexture(texqestion);
    qBox.setPosition(Pascal::scrWidth - texqestion.getSize().x - 5, 5);
    sf::Sprite aBox;
    aBox.setTexture(texanswer);
    aBox.setTextureRect({0, 0, static_cast<int>(texanswer.getSize().x), static_cast<int>(texanswer.getSize().y) / 5});

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

        window.draw(bg);
        window.draw(pascal);
        window.draw(qBox);

        if (this->_curQ >= Session::questionsPerSession) {
            this->_state = Pascal::State::Score;
            text.setFont(this->_mFont);
            this->_curQ = 0;
        }
        if (this->_state == Pascal::State::Menu) {
            text.setString(toWString(this->_files[idx]->getPath() + " (" + this->_files[idx]->getLanguage() + ")\n" + this->_files[idx]->getComment()));
            justifyText(text, {static_cast<int>(qBox.getPosition().x) + questionOffset, static_cast<int>(qBox.getPosition().y) + questionOffset, qBox.getTextureRect().width - 2 * questionOffset, qBox.getTextureRect().height - 2 * questionOffset});
            window.draw(text);
            text.setCharacterSize(30);
            validated = false;
            score = 0;
        } else if (this->_state == Pascal::State::Playing) {
            text.setString(toWString((*this->_files[idx])[this->_curQ].question));
            justifyText(text, {static_cast<int>(qBox.getPosition().x) + questionOffset, static_cast<int>(qBox.getPosition().y) + questionOffset, qBox.getTextureRect().width - 2 * questionOffset, qBox.getTextureRect().height - 2 * questionOffset});
            window.draw(text);
            text.setCharacterSize(30);
            for (std::size_t jdx = 0; jdx < (*this->_files[idx])[this->_curQ].propositions.size(); jdx++) {
                if (answer == jdx)
                    aBox.setTextureRect({0, aBox.getTextureRect().height, aBox.getTextureRect().width, aBox.getTextureRect().height});
                aBox.setPosition(qBox.getPosition().x + (qBox.getTextureRect().width - aBox.getTextureRect().width) * (jdx % 2), qBox.getTextureRect().height + 10 + (5 + aBox.getTextureRect().height) * (jdx / 2));
                window.draw(aBox);
                aBox.setTextureRect({0, 0, aBox.getTextureRect().width, aBox.getTextureRect().height});
                text.setString(toWString((*this->_files[idx])[this->_curQ].propositions[jdx]));
                justifyText(text, {static_cast<int>(aBox.getPosition().x) + answerOffset, static_cast<int>(aBox.getPosition().y) + answerOffset, aBox.getTextureRect().width - 2 * answerOffset, aBox.getTextureRect().height - 2 * answerOffset});
                window.draw(text);
                text.setCharacterSize(30);
            }
            if (validated) {
                if (answer == (*this->_files[idx])[this->_curQ].answer)
                    score++;
                this->_curQ++;
                answer = 0;
                validated = false;
            }
        } else if (this->_state == Pascal::State::Score) {
            text.setPosition(0, 0);
            text.setString(toWString("Score final: " + std::to_string(score) + "/10\n" + fMessages[score]));
            justifyText(text, {static_cast<int>(qBox.getPosition().x) + questionOffset, static_cast<int>(qBox.getPosition().y) + questionOffset, qBox.getTextureRect().width - 2 * questionOffset, qBox.getTextureRect().height - 2 * questionOffset});
            window.draw(text);
            text.setCharacterSize(30);
        }
        window.display();
        window.clear();
    }
}



void Pascal::getFiles(const std::string &dirPath)
{
    std::filesystem::path p(dirPath);

    for (const std::filesystem::directory_entry &dir_entry : std::filesystem::directory_iterator(p)) {
        if (dir_entry.is_regular_file() && endsWith(dir_entry.path(), ".qPUG")) {
            this->_files.push_back(std::make_unique<qPUG::Session>(dir_entry.path()));
            this->_files.back()->printAllQuestions();
        }
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
            answer = (answer + propositions.size() + 2) % propositions.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            answer = (answer + 2) % propositions.size();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            answer = ((answer / 2) * 2) + ((answer + 1) % 2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            answer = ((answer / 2) * 2) + ((answer + 3) % 2);
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
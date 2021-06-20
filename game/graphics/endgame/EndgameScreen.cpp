#include "EndgameScreen.hpp"
#include "../../util/CrashReporter.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    EndgameScreen::EndgameScreen(uint32_t gameEndStatus)
                                : Screen(ID_SCREEN_ENDGAME) {
        std::wcout << L"Endgame! Status: " << gameEndStatus << std::endl;
        window->setView(window->getDefaultView()); // сброс View

        // TODO сделать нормально

        sprite = std::make_unique<sf::Sprite>();

        switch (gameEndStatus) {
            default:
                CrashReporter::makeReport(
                        "unsupported game end status received: " + std::to_string(gameEndStatus));
                return;

            case 1: // SUCCESS
                sprite->setTexture(*Game::instance().getTextures()->endgameSuccess);
                Game::instance().getSoundSystem()->playMusic(Sound::ENDGAME_SUCCESS_THEME);

                break;

            case 2: // FAILURE_SOMEONE_DIED
                sprite->setTexture(*Game::instance().getTextures()->endgameFailureSomeoneDied);
                Game::instance().getSoundSystem()->playMusic(Sound::ENDGAME_FAILURE_THEME);

                break;

            case 9999: // SERVER_ERROR
                sprite->setTexture(*Game::instance().getTextures()->endgameServerError);
                Game::instance().getSoundSystem()->playMusic(Sound::ENDGAME_FAILURE_THEME);

                break;
        }

        sf::FloatRect baseBounds = sprite->getGlobalBounds();

        sprite->setScale( // устанавливаем нужный нам размер спрайта
                window->getSize().x / baseBounds.width, // NOLINT(cppcoreguidelines-narrowing-conversions)
                window->getSize().y / baseBounds.height // NOLINT(cppcoreguidelines-narrowing-conversions)
        );
    }

    void EndgameScreen::keyPressed(const sf::Event::KeyEvent& event) {
        Screen::keyPressed(event);
        Game::instance().returnToMainMenu(); // выход по нажатию любой клавиши
    }

    void EndgameScreen::update() {
        Screen::update();
    }

    void EndgameScreen::draw() {
        Screen::draw();
        window->draw(*sprite);
    }

}

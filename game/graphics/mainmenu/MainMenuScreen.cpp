// Кнопки
#define ALL_BUTTONS_X 70
#define BUTTONS_VERTICAL_MARGIN 5
#define EXTRA_BOTTOM_MARGIN 100
#define BUTTONS_WIDTH 700
#define BUTTONS_HEIGHT 70
// Логотип
#define LOGO_FONT_SIZE 85
#define LOGO_OUTLINE_THICKNESS 10
#define LOGO_EXTRA_LEFT_MARGIN 10
#define LOGO_VERTICAL_MARGIN 50

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include "MainMenuScreen.hpp"
#include "MainMenuButton.hpp"
#include "../ColorSet.hpp"
#include "../RenderUtils.hpp"
#include "../WaterBackground.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreen::createButtons() {
        // Кнопки отрисовываются снизу вверх, поэтому и добавляем их в "обратном" порядке.
        const unsigned int bMargin      = BUTTONS_VERTICAL_MARGIN * renderScale;
        const unsigned int bExtraMargin = EXTRA_BOTTOM_MARGIN     * renderScale;
        const unsigned int bWidth       = BUTTONS_WIDTH           * renderScale;
        const unsigned int bHeight      = BUTTONS_HEIGHT          * renderScale;
        const unsigned int bX           = ALL_BUTTONS_X           * renderScale;

        // QuitGame
        unsigned int bY = height - bMargin - bHeight - bExtraMargin;
        auto btnQuitGame = std::make_shared<MainMenuButton>(
                renderScale, window,
                L"Выйти из игры", bX, bY, bWidth, bHeight, quitGameClicked);
        children.push_back(btnQuitGame);

        // JoinLobby
        bY -= bMargin + bHeight;
        auto btnJoinLobby = std::make_shared<MainMenuButton>(
                renderScale, window,
                L"Присоединиться к комнате", bX, bY, bWidth, bHeight, joinLobbyClicked);
        children.push_back(btnJoinLobby);

        // CreateLobby
        bY -= bMargin + bHeight;
        auto btnCreateLobby = std::make_shared<MainMenuButton>(
                renderScale, window,
                L"Создать комнату", bX, bY, bWidth, bHeight, createLobbyClicked);
        children.push_back(btnCreateLobby);
    }

    void MainMenuScreen::createLobbyClicked() {
        std::cout << "create lobby" << std::endl;
    }

    void MainMenuScreen::joinLobbyClicked() {
        std::cout << "join lobby" << std::endl;
    }

    void MainMenuScreen::quitGameClicked() {
        std::cout << "quit game" << std::endl;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    MainMenuScreen::MainMenuScreen(float renderScale,
                                   const std::shared_ptr<sf::RenderWindow>& window) {
        this->renderScale = renderScale;
        this->window = window;

        x = 0;
        y = 0;
        width = window->getSize().x;
        height = window->getSize().y;

        children.push_back(std::make_shared<WaterBackground>(renderScale, window));
        createButtons();
    }

    void MainMenuScreen::update() {
        Drawable::update();
    }

    void MainMenuScreen::draw() {
        Drawable::draw();

        // Логотип.
        unsigned int logoFontSize   = LOGO_FONT_SIZE         * renderScale;
        unsigned int logoOutline    = LOGO_OUTLINE_THICKNESS * renderScale;
        unsigned int logoLeftMargin = LOGO_EXTRA_LEFT_MARGIN * renderScale;
        unsigned int allButtonsX    = ALL_BUTTONS_X          * renderScale;
        unsigned int logoVertMargin = LOGO_VERTICAL_MARGIN   * renderScale;
        unsigned int buttonsHeight  = BUTTONS_HEIGHT         * renderScale;

        unsigned int logoX          = allButtonsX + logoLeftMargin;
        unsigned int highestButtonY = children[children.size() - 1]->getY();
        unsigned int logoY          = highestButtonY - buttonsHeight - logoVertMargin;

        sf::Text logo;
        logo.setString(L"As We Drown");
        logo.setFont(*Game::instance().getFontManager()->getDecorativeFont());
        logo.setCharacterSize(logoFontSize);
        logo.setFillColor(ColorSet::LOGO_FILL);
        logo.setOutlineColor(ColorSet::LOGO_OUTLINE);
        logo.setOutlineThickness(logoOutline);
        logo.setPosition(logoX, logoY);
        window->draw(logo);
    }

}

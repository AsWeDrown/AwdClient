// ID
#define ID_BUTTON_QUIT_GAME 1
#define ID_BUTTON_JOIN_LOBBY 2
#define ID_BUTTON_CREATE_LOBBY 3
#define ID_WATER_BACKGROUND 100
#define ID_DIALOG_CREATE_LOBBY 1000
// Кнопки
#define ALL_BUTTONS_X 70
#define BUTTONS_VERTICAL_MARGIN 5
#define EXTRA_BOTTOM_MARGIN 100
#define BUTTONS_WIDTH 700
#define BUTTONS_HEIGHT 70
// Логотип
#define LOGO_FONT_SIZE 75
#define LOGO_OUTLINE_THICKNESS 10
#define LOGO_EXTRA_LEFT_MARGIN 10
#define LOGO_VERTICAL_MARGIN 50
#define LETTER_SPACING_FACTOR 1.1f

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include "MainMenuScreen.hpp"
#include "../common/TextButton.hpp"
#include "../common/ColorSet.hpp"
#include "../common/RenderUtils.hpp"
#include "../common/WaterBackground.hpp"
#include "../../Game.hpp"
#include "../common/TextInputDialog.hpp"

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
        auto btnQuitGame = std::make_shared<TextButton>(
                ID_BUTTON_QUIT_GAME, renderScale, window,
                L"Выйти из игры", bX, bY, bWidth, bHeight, quitGameClicked);
        addChild(btnQuitGame);

        // JoinLobby
        bY -= bMargin + bHeight;
        auto btnJoinLobby = std::make_shared<TextButton>(
                ID_BUTTON_JOIN_LOBBY, renderScale, window,
                L"Присоединиться к комнате", bX, bY, bWidth, bHeight, joinLobbyClicked);
        addChild(btnJoinLobby);

        // CreateLobby
        bY -= bMargin + bHeight;
        auto btnCreateLobby = std::make_shared<TextButton>(
                ID_BUTTON_CREATE_LOBBY, renderScale, window,
                L"Создать комнату", bX, bY, bWidth, bHeight, createLobbyClicked);
        addChild(btnCreateLobby);
    }

    void MainMenuScreen::dialogOpened(Drawable* mainMenuScreen, int dialogId) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->dialogOpen = true;
    }

    void MainMenuScreen::dialogClosed(Drawable* mainMenuScreen, int dialogId) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->dialogOpen = false;
        mainMenu->removeChild(dialogId);
    }

    void MainMenuScreen::createLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;

        if (mainMenu->dialogOpen)
            return;

        auto dialog = std::make_shared<TextInputDialog>(
                ID_DIALOG_CREATE_LOBBY, mainMenu->renderScale, mainMenu->window,
                dialogOpened, dialogClosed,
                createLobbyNextClicked, createLobbyBackClicked
        );

        mainMenu->addChild(dialog);
        dialog->show();
    }

    void MainMenuScreen::createLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput) {
        std::wcout << L"MainMenu : CreateLobby : next -- " << userInput << std::endl;
        //todo
    }

    void MainMenuScreen::createLobbyBackClicked(Drawable* mainMenuScreen) {
        auto dialog = std::dynamic_pointer_cast<Dialog>(
                mainMenuScreen->getChildById(ID_DIALOG_CREATE_LOBBY));

        dialog->hide();
    }

    void MainMenuScreen::joinLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;

        if (mainMenu->dialogOpen)
            return;

        //todo
    }

    void MainMenuScreen::joinLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput) {
        //todo
    }

    void MainMenuScreen::joinLobbyBackClicked(Drawable* mainMenuScreen) {
        //todo
    }

    void MainMenuScreen::quitGameClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;

        if (mainMenu->dialogOpen)
            return;

        //todo
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

        addChild(std::make_shared<WaterBackground>(
                ID_WATER_BACKGROUND, renderScale, window));
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
        unsigned int highestButtonY = getChildById(ID_BUTTON_CREATE_LOBBY)->getY();
        unsigned int logoY          = highestButtonY - buttonsHeight - logoVertMargin;

        sf::Text logo;
        logo.setString(L"As We Drown");
        logo.setFont(*Game::instance().getFontManager()->getDecorativeFont());
        logo.setCharacterSize(logoFontSize);
        logo.setLetterSpacing(LETTER_SPACING_FACTOR);
        logo.setFillColor(ColorSet::LOGO_FILL);
        logo.setOutlineColor(ColorSet::LOGO_OUTLINE);
        logo.setOutlineThickness(logoOutline);
        logo.setPosition(logoX, logoY);
        window->draw(logo);
    }

}

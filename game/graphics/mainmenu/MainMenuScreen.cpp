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
                ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME, renderScale, window,
                L"Выйти из игры", bX, bY, bWidth, bHeight, listener);
        addButton(btnQuitGame);

        // JoinLobby
        bY -= bMargin + bHeight;
        auto btnJoinLobby = std::make_shared<TextButton>(
                ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY, renderScale, window,
                L"Присоединиться к комнате", bX, bY, bWidth, bHeight, listener);
        addButton(btnJoinLobby);

        // CreateLobby
        bY -= bMargin + bHeight;
        auto btnCreateLobby = std::make_shared<TextButton>(
                ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY, renderScale, window,
                L"Создать комнату", bX, bY, bWidth, bHeight, listener);
        addButton(btnCreateLobby);
    }

    void MainMenuScreen::addButton(const std::shared_ptr<Drawable>& button) {
        addChild(button);
        buttonsIds.push_back(button->getId());
    }

    void MainMenuScreen::setButtonsEnabled(bool enabled) {
        for (id_type buttonId : buttonsIds)
            getChildById(buttonId)->setEnabled(enabled);
    }


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    MainMenuScreen::MainMenuScreen(float renderScale,
                                   const std::shared_ptr<sf::RenderWindow>& window)
                                   : Drawable(ID_SCREEN_MAIN_MENU, renderScale, window) {
        x = 0;
        y = 0;
        width = window->getSize().x;
        height = window->getSize().y;

        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_MAIN_MENU_WATER_BACKGROUND, renderScale, window));
        createButtons();
    }

    void MainMenuScreen::update() {
        // Выключаем все кнопки меню, если открыто окно (Dialog).
        setButtonsEnabled(!dialogOpen);

        // Обновляем все дочерние компоненты (в т.ч. кнопки) только после проверки условий выше.
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
        unsigned int highestButtonY = getChildById(ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY)->getY();
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

    bool MainMenuScreen::isDialogOpen() const {
        return dialogOpen;
    }

    void MainMenuScreen::setDialogOpen(bool open) {
        this->dialogOpen = open;
    }

    std::shared_ptr<MainMenuScreenListener> MainMenuScreen::getListener() const {
        return listener;
    }

}

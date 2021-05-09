// Кнопки
#define ALL_BUTTONS_X 70.0f
#define BUTTONS_VERTICAL_MARGIN 5.0f
#define EXTRA_BOTTOM_MARGIN 100.0f
#define BUTTONS_WIDTH 700.0f
#define BUTTONS_HEIGHT 70.0f
// Логотип
#define LOGO_FONT_SIZE 75
#define LOGO_OUTLINE_THICKNESS 10.0f
#define LOGO_EXTRA_LEFT_MARGIN 10.0f
#define LOGO_VERTICAL_MARGIN 50.0f
#define LETTER_SPACING_FACTOR 1.1f


#include <SFML/Graphics/RectangleShape.hpp>
#include <thread>
#include "MainMenuScreen.hpp"
#include "../common/TextButton.hpp"
#include "../common/ColorSet.hpp"
#include "../../util/RenderUtils.hpp"
#include "../common/WaterBackground.hpp"
#include "../../Game.hpp"
#include "../common/ErrorDialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreen::createButtons() {
        // Кнопки отрисовываются снизу вверх, поэтому и добавляем их в "обратном" порядке.
        const float bMargin      = BUTTONS_VERTICAL_MARGIN * renderScale;
        const float bExtraMargin = EXTRA_BOTTOM_MARGIN     * renderScale;
        const float bWidth       = BUTTONS_WIDTH           * renderScale;
        const float bHeight      = BUTTONS_HEIGHT          * renderScale;
        const float bX           = ALL_BUTTONS_X           * renderScale;

        // QuitGame
        float bY = height - bMargin - bHeight - bExtraMargin;
        auto btnQuitGame = std::make_shared<TextButton>(
                ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME, renderScale, window,
                L"Выйти из игры", bX, bY, bWidth, bHeight, listener);
        addComponent(btnQuitGame);

        // JoinLobby
        bY -= bMargin + bHeight;
        auto btnJoinLobby = std::make_shared<TextButton>(
                ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY, renderScale, window,
                L"Присоединиться к комнате", bX, bY, bWidth, bHeight, listener);
        addComponent(btnJoinLobby);

        // CreateLobby
        bY -= bMargin + bHeight;
        auto btnCreateLobby = std::make_shared<TextButton>(
                ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY, renderScale, window,
                L"Создать комнату", bX, bY, bWidth, bHeight, listener);
        addComponent(btnCreateLobby);

        // Нужно для размещения логотипа
        highestButtonY = bY;
    }


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    MainMenuScreen::MainMenuScreen(float renderScale,
                                   const std::shared_ptr<sf::RenderWindow>& window)
                                   : Screen(ID_SCREEN_MAIN_MENU, renderScale, window) {
        x      = 0.0f;
        y      = 0.0f;
        width  = window->getSize().x;
        height = window->getSize().y;

        // Фон
        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_MAIN_MENU_WATER_BACKGROUND, renderScale, window));

        // Кнопки
        createButtons();

        // Логотип
        unsigned int logoFontSize   = LOGO_FONT_SIZE         * renderScale;
        float        logoOutline    = LOGO_OUTLINE_THICKNESS * renderScale;
        float        logoLeftMargin = LOGO_EXTRA_LEFT_MARGIN * renderScale;
        float        allButtonsX    = ALL_BUTTONS_X          * renderScale;
        float        logoVertMargin = LOGO_VERTICAL_MARGIN   * renderScale;
        float        buttonsHeight  = BUTTONS_HEIGHT         * renderScale;
        float        logoX          = allButtonsX + logoLeftMargin;
        float        logoY          = highestButtonY - buttonsHeight - logoVertMargin;

        logo = std::make_unique<sf::Text>();
        logo->setFont(*Game::instance().getFontManager()->getDecorativeFont());
        logo->setString(L"As We Drown");
        logo->setCharacterSize(logoFontSize);
        logo->setLetterSpacing(LETTER_SPACING_FACTOR);
        logo->setFillColor(ColorSet::LOGO_FILL);
        logo->setOutlineColor(ColorSet::LOGO_OUTLINE);
        logo->setOutlineThickness(logoOutline);
        logo->setPosition(logoX, logoY);

        std::wcerr << L"logo pos: " << logoX << L" / " << logoY << std::endl;
    }

    void MainMenuScreen::update() {
        Screen::update();
    }

    void MainMenuScreen::draw() {
        Screen::draw();
        window->draw(*logo);
    }

    std::shared_ptr<MainMenuScreenListener> MainMenuScreen::getListener() const {
        return listener;
    }

    WorkflowState MainMenuScreen::getWorkflowState() const {
        return workflowState;
    }

    void MainMenuScreen::setWorkflowState(WorkflowState state) {
        this->workflowState = state;
    }

    void MainMenuScreen::showLoadingOverlay(const std::wstring& loadingMessage,
                                            unsigned int timeoutMillis) {
        auto loadingOverlay = std::make_shared<LoadingOverlay>(
                ID_SCREEN_MAIN_MENU_LOADING_OVERLAY,
                renderScale, window, loadingMessage);

        Screen::showLoadingOverlay(loadingOverlay);

        std::thread timeoutThread([this, loadingMessage, timeoutMillis]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMillis));
            auto currentLoadingOverlay = getChildById(currentLoadingOverlayId);

            if (currentLoadingOverlay != nullptr) {
                std::wstring currentLoadingMessage = std::dynamic_pointer_cast
                        <LoadingOverlay>(currentLoadingOverlay)->getUnformattedMessage();

                if (currentLoadingMessage == loadingMessage) {
                    // Истекло время ожидания (hideCurrentLoadingOverlay
                    // для ЭТОГО сообщения загрузки так и не был вызван).
                    hideCurrentLoadingOverlay(); // скрываем сообщение о загрузке (оно больше не нужно), ...
                    closeCurrentDialog(); // ... закрываем текущий диалог, каким бы он не был, ...

                    auto dialog = std::make_shared<ErrorDialog>(
                            ID_SCREEN_MAIN_MENU_DIALOG_ERROR,
                            renderScale,
                            window,
                            listener,
                            L"{RED}{BOLD}Ошибка: "
                            L"{RESET}{GRAY}истекло максимальное допустимое время ожидания. "
                            L"Проверьте своё подключение к Интернету.",
                            ID_SCREEN_MAIN_MENU_DIALOG_ERROR_BUTTON_OK,
                            listener
                    );

                    openDialog(dialog); // ... и отображаем диалог с ошибкой об истечении времени ожидания.
                }
            }
        });

        timeoutThread.detach();
    }

    void MainMenuScreen::showErrorDialog(const std::wstring& message) {
        auto dialog = std::make_shared<ErrorDialog>(
                ID_SCREEN_MAIN_MENU_DIALOG_ERROR,
                renderScale,
                window,
                listener,
                message,
                ID_SCREEN_MAIN_MENU_DIALOG_ERROR_BUTTON_OK,
                listener
        );

        openDialog(dialog);
    }

}

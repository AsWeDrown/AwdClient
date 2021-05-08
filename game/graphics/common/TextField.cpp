// Общие
#define BASE_FILL_ALPHA 80
#define BASE_LINE_ALPHA 125
#define BASE_LINE_WIDTH 3.0f /* имеется в виду высота прямоугольника (height) */
// Эффекты выделения
#define MAX_EFFECTIVE_SELECTED_TICKS 3
#define BONUS_FILL_ALPHA_PER_SELECTED_TICK 15
#define BONUS_LINE_ALPHA_PER_SELECTED_TICK 30
#define BONUS_LINE_WIDTH_PER_SELECTED_TICK 0.5f /* имеется в виду высота прямоугольника (height) */
// Текст
#define TEXT_FONT_SIZE 45
#define TEXT_TOP_MARGIN 18.0f
#define TEXT_LEFT_MARGIN 25.0f
#define HINT_TEXT_ALPHA 140
// Макросы для упрощения огромной switch/case конструкции для преобразования кодов клавиш в символы на них
#define KEY(x) case sf::Keyboard::x :                                            \
                   charTyped = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)  \
                           ? #x[0] : std::tolower(#x[0]);                        \
                   break;
#define NUM(x) case sf::Keyboard::Num ##x :    \
               case sf::Keyboard::Numpad ##x:  \
                   charTyped = #x[0];          \
                   break;


#include "TextField.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TextField::TextField(id_type id,
                         float renderScale,
                         const std::shared_ptr<sf::RenderWindow>& window,
                         float x, float y,
                         float width, float height,
                         unsigned int maxContentsLen,
                         const std::wstring& hintText,
                         const std::wstring& initialContents,
                         const std::shared_ptr<TextFieldListener>& listener)
                         : Drawable(id, renderScale, window) {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;
        this->maxContentsLen = maxContentsLen;
        this->hintText = hintText;
        this->contents = initialContents.length() <= maxContentsLen
                ? initialContents : initialContents.substr(0, maxContentsLen);
        this->listener = listener;

        // Закрашивание
        tfFill = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        tfFill->setPosition(x, y);
        tfFill->setFillColor(sf::Color::Transparent);

        // Горизонтальная черта снизу
        float baseWidth = BASE_LINE_WIDTH * renderScale + 1.0f; // min 1 px

        tfLine = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, baseWidth));
        tfLine->setPosition(x, y + height - baseWidth); // минус lineWidth, чтобы черта "росла" вверх (внутрь поля)
        tfLine->setFillColor(sf::Color::Transparent);

        // Введённый пользователем текст (или hintText (подсказка), если пользователь ничего не ввёл)
        unsigned int fontSize   = TEXT_FONT_SIZE   * renderScale;
        float        topMargin  = TEXT_TOP_MARGIN  * renderScale;
        float        leftMargin = TEXT_LEFT_MARGIN * renderScale;

        tfText = std::make_unique<sf::Text>();
        tfText->setFont(*Game::instance().getFontManager()->getRegularFont());
        tfText->setString(contents.empty() ? hintText : contents);
        tfText->setCharacterSize(fontSize);
        tfText->setFillColor(sf::Color(255, 255, 255,
                                    contents.empty() ? HINT_TEXT_ALPHA : 255));
        tfText->setPosition(x + leftMargin, y + topMargin);
    }

    void TextField::keyPressed(const sf::Event::KeyEvent& event) {
        if (!selected)
            return;

        // TODO: возможность перемещать "курсор" стралочками <влево/вправо>.

        if (event.code == sf::Keyboard::Backspace) {
            if (!contents.empty())
                // Возможность стирать введённые символы.
                setContents(contents.substr(0, contents.length() - 1));
        } else if (contents.length() < maxContentsLen) {
            wchar_t charTyped;

            switch (event.code) {
                // Латиница (строчные и прописные буквы)
                KEY(A) KEY(B) KEY(C) KEY(D) KEY(E)
                KEY(F) KEY(G) KEY(H) KEY(I) KEY(J)
                KEY(K) KEY(L) KEY(M) KEY(N) KEY(O)
                KEY(P) KEY(Q) KEY(R) KEY(S) KEY(T)
                KEY(U) KEY(V) KEY(W) KEY(X) KEY(Y)
                              KEY(Z)

                // Цифры
                NUM(0) NUM(1) NUM(2) NUM(3) NUM(4)
                NUM(5) NUM(6) NUM(7) NUM(8) NUM(9)

                // Символ "подчёркивания" (underscore)
                case sf::Keyboard::Hyphen:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                        charTyped = L'_'; // '_' можно
                    else
                        charTyped = 0; // а вот '-' нельзя

                    break;

                // Какая-то левая кнопка - игнорируем
                default:
                    charTyped = 0;
                    break;
            }

            if (charTyped != 0)
                setContents(contents + charTyped);
        }
    }

    void TextField::mousePressed(const sf::Event::MouseButtonEvent& event) {
        selected = isMouseOver();
    }

    void TextField::update() {
        Drawable::update();

        if (selected) {
            if (selectedTicks < MAX_EFFECTIVE_SELECTED_TICKS)
                selectedTicks++;
        } else if (selectedTicks > 0)
            selectedTicks--;

        // Закрашивание
        unsigned int fillAlpha = BASE_FILL_ALPHA + BONUS_FILL_ALPHA_PER_SELECTED_TICK * selectedTicks;
        tfFill->setFillColor(sf::Color(0, 0, 0, fillAlpha));

        // Горизонтальная черта снизу
        float        baseWidth = BASE_LINE_WIDTH                    * renderScale + 1.0f; // min 1 px
        float        bwPerTick = BONUS_LINE_WIDTH_PER_SELECTED_TICK * renderScale + 1.0f; // min 1 px
        float        lineWidth = baseWidth + bwPerTick * selectedTicks;
        unsigned int lineAlpha = BASE_LINE_ALPHA + BONUS_LINE_ALPHA_PER_SELECTED_TICK * selectedTicks;

        tfLine->setFillColor(sf::Color(255, 255, 255, lineAlpha));
        tfLine->setSize(sf::Vector2f(tfLine->getSize().x, lineWidth));
        tfLine->setPosition(x, y + height - lineWidth); // минус lineWidth, чтобы черта "росла" вверх (внутрь поля)

        // Введённый пользователем текст (или hintText (подсказка), если пользователь ничего не ввёл)
        tfText->setString(contents.empty() ? hintText : contents);
        tfText->setFillColor(sf::Color(255, 255, 255,
                                       contents.empty() ? HINT_TEXT_ALPHA : 255));
    }

    void TextField::draw() {
        Drawable::draw();

        window->draw(*tfFill);
        window->draw(*tfLine);
        window->draw(*tfText);
    }

    std::wstring TextField::getContents() const {
        return contents;
    }

    void TextField::setContents(const std::wstring& newContents) {
        listener->contentsChanged(parent, id, this->contents = newContents);
    }

    bool TextField::isSelected() const {
        return selected;
    }

    void TextField::setSelected(bool nowSelected) {
        this->selected = nowSelected;
    }

}

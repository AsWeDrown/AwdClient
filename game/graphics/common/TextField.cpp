// Общие
#define BASE_FILL_ALPHA 80
#define BASE_LINE_ALPHA 125
#define BASE_LINE_WIDTH 3 /* имеется в виду высота прямоугольника (height) */
// Эффекты выделения
#define MAX_EFFECTIVE_SELECTED_TICKS 3
#define EXTRA_FILL_ALPHA_PER_SELECTED_TICK 15
#define EXTRA_LINE_ALPHA_PER_SELECTED_TICK 30
#define EXTRA_LINE_WIDTH_PER_SELECTED_TICK 1 /* имеется в виду высота прямоугольника (height) */
// Текст
#define TEXT_FONT_SIZE 45
#define TEXT_TOP_MARGIN 20
#define TEXT_LEFT_MARGIN 25
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
                         unsigned int x, unsigned int y,
                         unsigned int width, unsigned int height,
                         unsigned int maxContentsLen,
                         const std::wstring& hintText,
                         const std::shared_ptr<TextFieldListener>& listener)
                         : Drawable(id, renderScale, window) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->maxContentsLen = maxContentsLen;
        this->hintText = hintText;
        this->listener = listener;
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
                KEY(A)
                KEY(B)
                KEY(C)
                KEY(D)
                KEY(E)
                KEY(F)
                KEY(G)
                KEY(H)
                KEY(I)
                KEY(J)
                KEY(K)
                KEY(L)
                KEY(M)
                KEY(N)
                KEY(O)
                KEY(P)
                KEY(Q)
                KEY(R)
                KEY(S)
                KEY(T)
                KEY(U)
                KEY(V)
                KEY(W)
                KEY(X)
                KEY(Y)
                KEY(Z)

                // Цифры
                NUM(0)
                NUM(1)
                NUM(2)
                NUM(3)
                NUM(4)
                NUM(5)
                NUM(6)
                NUM(7)
                NUM(8)
                NUM(9)

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
    }

    void TextField::draw() {
        Drawable::draw();

        unsigned int fillAlpha = BASE_FILL_ALPHA + EXTRA_FILL_ALPHA_PER_SELECTED_TICK * selectedTicks;
        unsigned int lineAlpha = BASE_LINE_ALPHA + EXTRA_LINE_ALPHA_PER_SELECTED_TICK * selectedTicks;
        unsigned int baseWidth = BASE_LINE_WIDTH                                      * renderScale + 1; // min 1 px
        unsigned int ewPerTick = EXTRA_LINE_WIDTH_PER_SELECTED_TICK                   * renderScale + 1; // min 1 px
        unsigned int lineWidth = baseWidth + ewPerTick * selectedTicks;

        // Закрашивание
        sf::RectangleShape fill(sf::Vector2f(width, height));
        fill.setPosition(x, y);
        fill.setFillColor(sf::Color(0, 0, 0, fillAlpha));
        window->draw(fill);

        // Горизонтальная черта снизу
        sf::RectangleShape line(sf::Vector2f(width, lineWidth));
        line.setPosition(x, y + height - lineWidth); // минус lineWidth, чтобы черта "росла" вверх (внутрь поля)
        line.setFillColor(sf::Color(255, 255, 255, lineAlpha));
        window->draw(line);

        // Введённый пользователем текст (или hintText (подсказка), если пользователь ничего не ввёл)
        unsigned int fontSize   = TEXT_FONT_SIZE   * renderScale;
        unsigned int topMargin  = TEXT_TOP_MARGIN  * renderScale;
        unsigned int leftMargin = TEXT_LEFT_MARGIN * renderScale;

        sf::Text text;
        text.setFont(*Game::instance().getFontManager()->getRegularFont());
        text.setString(contents.empty() ? hintText : contents);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color(255, 255, 255,
                                    contents.empty() ? HINT_TEXT_ALPHA : 255));
        text.setPosition(x + leftMargin, y + topMargin);
        window->draw(text);
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

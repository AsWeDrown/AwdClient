#pragma once


#include "Drawable.hpp"
#include "TextFieldListener.hpp"

namespace awd::game {

    class TextField : public Drawable {
    protected:
        std::shared_ptr<TextFieldListener> listener = nullptr;
        unsigned int maxContentsLen;
        std::wstring hintText;
        std::wstring contents;
        int selectedTicks = 0;
        bool selected = false;

    public:
        TextField(id_type id,
                  float renderScale,
                  const std::shared_ptr<sf::RenderWindow>& window,
                  unsigned int x, unsigned int y,
                  unsigned int width, unsigned int height,
                  unsigned int maxContentsLen,
                  const std::wstring& hintText,
                  const std::shared_ptr<TextFieldListener>& listener);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        std::wstring getContents() const;
        void setContents(const std::wstring& newContents);
        bool isSelected() const;
        void setSelected(bool nowSelected);
    };

}


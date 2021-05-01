#pragma once


#include <string>
#include "Drawable.hpp"
#include "ButtonListener.hpp"

namespace awd::game {

    // IDE может тут показывать ошибку "redefinition of class". Игнорируем - это из-за
    // того, что в самом низу файла включён файл ".ipp" (реализация шаблонных функций).
    // На самом деле всё компилируется и собирается нормально, без ошибок (баг IDE?).
    template<typename TPayloadType>
    class Button : public Drawable {
    protected:
        std::shared_ptr<ButtonListener<TPayloadType>> listener = nullptr;
        TPayloadType payload;

    public:
        Button(id_type id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window,
               const std::shared_ptr<ButtonListener<TPayloadType>>& listener);

        void update() override;
        void draw() override;
        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;

        TPayloadType getPayload() const;
        void setPayload(const TPayloadType& newPayload);
    };

}

// Нужно для работы шаблонных функций (иначе не скомпилируется):
// https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
// Файл реализации при этом лучше не называть ".cpp", т.к. по сути он - тоже заголовок.
#include "Button.ipp"

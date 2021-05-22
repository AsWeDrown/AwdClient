#pragma once


namespace awd::game {

    class GameState {
    public:
        /**
         * Используется только в клиенте. Сигнализирует о необходимости закрытия игры.
         */
        static constexpr uint32_t EXIT  = 9999;

        /**
         * Этап аутентификации/рукопожатия.
         */
        static constexpr uint32_t AUTH  = 0;

        /**
         * Предыгровой этап.
         */
        static constexpr uint32_t LOBBY = 1;

        /**
         * Игровой этап.
         */
        static constexpr uint32_t PLAY  = 2;
    };

}

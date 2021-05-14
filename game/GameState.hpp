#pragma once


namespace awd::game {

    class GameState {
    public:
        /**
         * Используется только в клиенте. Сигнализирует о необходимости закрытия игры.
         */
        static const uint32_t EXIT  = 9999;

        /**
         * Этап аутентификации/рукопожатия.
         */
        static const uint32_t AUTH  = 0;

        /**
         * Предыгровой этап.
         */
        static const uint32_t LOBBY = 1;

        /**
         * Игровой этап.
         */
        static const uint32_t PLAY  = 2;
    };

}

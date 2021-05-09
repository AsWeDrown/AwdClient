#pragma once


namespace awd::game {

    class GameState {
    public:
        /**
         * Используется только в клиенте. Сигнализирует о необходимости закрытия игры.
         */
        static const uint32_t EXIT  = -1;

        /**
         * Предыгровой этап.
         */
        static const uint32_t LOBBY = 0;

        /**
         * Игровой этап.
         */
        static const uint32_t PLAY  = 1;
    };

}

#pragma once


namespace awd::game {

    class GameState {
    public:
        /**
         * Используется только в клиенте. Сигнализирует о необходимости закрытия игры.
         */
        static const int EXIT  = -1;

        /**
         * Предыгровой этап.
         */
        static const int LOBBY = 0;

        /**
         * Игровой этап.
         */
        static const int PLAY  = 1;
    };

}

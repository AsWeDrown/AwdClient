#pragma once


namespace awd::game {

    class GameState {
    public:
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

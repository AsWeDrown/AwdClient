#include "game/Game.hpp"

int main() {
    //TODO (1) обернуть в try/catch, краш-репорты, логи; (2) оптимизировать #include везде; (3) updater (?).
    awd::game::Game::instance().bootstrap();
}
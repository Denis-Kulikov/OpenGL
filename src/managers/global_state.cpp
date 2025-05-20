#include <managers/global_state.hpp> 

void GlobalState::TerminateApp() {
    isAppRunning = false;
}

void GlobalState::SetPlayer(Pawn* newPlayer) {
    player = newPlayer;
}

Pawn* GlobalState::GetPlayer() {
    return player;
}

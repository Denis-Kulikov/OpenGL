#include <managers/global_state.hpp> 

void GlobalState::TerminateApp() {
    isAppRunning = false;
}

void GlobalState::SetPlayer(Actor* newPlayer) {
    player = newPlayer;
}

Actor* GlobalState::GetPlayer() {
    return player;
}

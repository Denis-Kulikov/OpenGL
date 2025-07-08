#include <managers/global_state.hpp> 

void GlobalState::TerminateApp() {
    fIsAppRunning = false;
}

void GlobalState::SetPlayer(Actor* newPlayer) {
    player = newPlayer;
}

Actor* GlobalState::GetPlayer() {
    return player;
}

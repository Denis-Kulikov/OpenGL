#include <managers/global.hpp> 

void GlobalState::TerminateApp() {
    fIsAppRunning = false;
}

void GlobalState::SetPlayer(Actor* newPlayer) {
    player = newPlayer;
}

Actor* GlobalState::GetPlayer() {
    return player;
}

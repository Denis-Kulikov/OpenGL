#include <game/gameManager.hpp> 
#include <game/gameTime.hpp>
#include <server/server.hpp>

GameTime Time;

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -16.0, MAX_X = 16.0;
    float MIN_Y = -2.0, MAX_Y = 2.0;
    float MIN_Z = -16.0, MAX_Z = 2.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}



int main(int argc, char** argv)
{
    Server server;
    server.callback();

    return 0;
}

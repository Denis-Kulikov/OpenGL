#pragma once
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <functional>
#include <list>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>

#include <chrono>
#include <ctime>
#include <random>
#include <thread>
// #include <pthread.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math_3d.hpp"

#define MAX_PLAYERS 6
#define MAX_SIZE 64
#define PORT 8888
#define SERVER_IP "127.0.0.1"

#define PLAYER_SPEED 0.3
#define MAX_HP 100
#define BULLET_DAMAGE 100 / 4 

enum {
    CLIENT_CONNECT,
    CLIENT_DISCONNECT,

    MOVE,
    FIRE,
    // KILL,
    // REVIVAL,
    // TELEPORT,

    SET_HP

    // DAMAGE,
    // HEALING,
};

struct MOVE_INFO {
    int code_message;
    Vector3<float> position;
    float direction;
};

struct FIRE_INFO {
    int code_message;
    Vector3<float> position;
    float direction;
};

// struct KILL_INFO {
//     int code_message;
//     int id;
// }

// struct REVIVAL_INFO {
//     int code_message;
//     int id;
// }

struct SET_HP_INFO {
    int code_message;
    int HP;
    int id;
};


struct ID_MOVE_INFO {
    int code_message;
    Vector3<float> position;
    int id;
};

struct ID_FIRE_INFO {
    // struct ID_FIRE_INFO fire_info;
    int id;
};

using message_type = int;

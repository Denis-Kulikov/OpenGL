#pragma once
#include "../entities/actor.hpp"
#include "../render/render.hpp"
#include "../object/scene.hpp"

#define THREADS_SLEEP_TIME_MS 3

class RenderThread {
public:
    RenderThread(std::atomic<bool>* endTickPtr, std::size_t numComponentsThreads);
    void pushSprite(const std::pair<Matrix4f<GLfloat>, Sprite*>& ComponentSprite, std::size_t n);
    bool empty();
    void job();
    void setEnd(std::size_t n);

    std::atomic<bool>* sceneEndTickPtr = nullptr;
// private:
    void callback();
    void popActor();
    void swapBuffer();

    std::stack<std::pair<Matrix4f<GLfloat>, Sprite*>> sprites[1];
    std::mutex mutex[1];
    std::size_t num = 0;
    std::atomic<bool> endTicks[1];

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
    std::chrono::duration<double> swapDuration;

};


class ComponentsThread {
public:
    ComponentsThread();
    void init(std::size_t n, RenderThread* renderThreadPtr);
    void job();
    void pushActor(const Actor* actor);
    bool empty();
    void setEnd();

// private:
    void callback();

    std::stack<const Actor*> actors;
    std::mutex mutex;
    std::atomic<bool> endTick = false;
    std::size_t num = 0;
    RenderThread* renderThread;

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
};

class SceneThread {
public:
    SceneThread();
    SceneThread(std::size_t numComponentsThreads);
    void start();
    void setScene(const Scene* _scene);
    void job();

private:
    void callback();
    const Scene* scene = nullptr;
    ComponentsThread componentsThreads[1];
    RenderThread renderThread;
    std::atomic<bool> endTick;
    std::size_t numThreads;
    std::mutex mutex;
    std::size_t numThread = 0;

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
};

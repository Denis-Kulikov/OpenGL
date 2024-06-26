#pragma once
#include "../entities/actor.hpp"
#include "../render/render.hpp"
#include "../object/scene.hpp"

#define THREADS_SLEEP_TIME_MS 1

class RenderThread {
public:
    RenderThread(std::atomic<bool>* endTickPtr);
    void pushComponents(const std::vector<Component*>& ActorComponents);
    bool empty();
    void job();

    std::atomic<bool> endTick = false;
// private:
    void callback();
    void popActor();
    void swapBuffer();

    std::stack<std::vector<Component*>> components;
    std::mutex mutex;
    std::atomic<bool>* sceneEndTickPtr = nullptr;
};


class ComponentsThread {
public:
    ComponentsThread(std::atomic<bool>* endTickPtr);
    void setScene(const Scene* _scene);
    void pushActor(const Actor* actor);
    bool empty();
    void job();

    std::atomic<bool> endTick = false;
// private:
    void callback();

    RenderThread renderThread;
    std::stack<const Actor*> actors;
    std::mutex mutex;
};

class SceneThread {
public:
    SceneThread();
    void start();
    void setScene(const Scene* _scene);
    void job();

private:
    void callback();
    const Scene* scene = nullptr;
    ComponentsThread componentsThread;
    std::mutex mutex;
    std::atomic<bool> endTick = false;
};
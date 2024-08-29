#pragma once
#include "../entities/actor.hpp"
#include "../render/render.hpp"
#include "../object/scene.hpp"
#include "../binary-tree/binary_tree.hpp"

#define THREADS_SLEEP_TIME_MS 1
#define THREAD_SCENE 3
#define THREAD_COMPONENTS 1
#define THREAD_RENDER 1

class RenderThread {
public:
    RenderThread(std::atomic<bool>* endTickPtr);
    void pushSprite(const std::pair<Matrix4f<GLfloat>, Sprite*>& ComponentSprite);
    void job();
    void setEnd();

    void callback();
private:
    void popActor();
    void swapBuffer();

    std::stack<std::pair<Matrix4f<GLfloat>, Sprite*>> sprites;
    std::mutex mutex;
    std::atomic<bool> endTicks;
    std::atomic<bool>* sceneEndTickPtr = nullptr;

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
    std::chrono::duration<double> swapDuration;
};


class ComponentsThread {
public:
    ComponentsThread(std::atomic<bool>* endTickPtr);
    void job();
    void pushActor(Actor* actor);
    void setEnd();

    RenderThread renderThread;
private:
    void callback();

    std::stack<Actor*> actors;
    std::mutex mutex;
    std::atomic<bool> endTick = false;

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
};

class SceneThread {
public:
    SceneThread();
    void start();
    void setScene(const Scene* _scene);
    void drowNode(objectTransform transform, const std::string &text);
    void drowBinaryTree(const objectTransform &transform, const tree_node<std::string> &node);
    void job();

private:
    void callback();
    const Scene* scene = nullptr;
    RenderThread *renderThread = nullptr;
    ComponentsThread componentsThread;
    std::atomic<bool> endTick;
    std::mutex mutex;

    std::chrono::high_resolution_clock::time_point startWorkTime;
    std::chrono::high_resolution_clock::time_point endWorkTime;
    std::chrono::duration<double> workDuration;
    std::chrono::duration<double> idleDuration;
};

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 


RenderThread::RenderThread(std::atomic<bool>* endTickPtr)
    : sceneEndTickPtr(endTickPtr)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::job() {
    while (!GameManager::IsEnd) callback();
}

void RenderThread::callback() {
        if (components.empty()) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
        if (endTick) {
            endTick = false;
            *sceneEndTickPtr = true;
            swapBuffer();
        }
    } else {
        mutex.lock();
        const std::vector<Component*> component = components.top();
        components.pop();
        mutex.unlock();

        for (const auto& it : component)
            GameManager::render->drawObject(&it->transform, it->sprite);
    }
}

void RenderThread::swapBuffer()
{
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::pushComponents(const std::vector<Component*>& ActorComponents) {
    std::lock_guard<std::mutex> lock(mutex);
    components.push(ActorComponents);
}

bool RenderThread::empty() {
    std::lock_guard<std::mutex> lock(mutex);
    return components.empty();
}

void RenderThread::setEnd()
{
    endTick = true;
}

// Реализация методов ComponentsThread

ComponentsThread::ComponentsThread(std::atomic<bool>* endTickPtr)
    : renderThread(endTickPtr)
{}

void ComponentsThread::job() {
    while (!GameManager::IsEnd) callback();
}

void ComponentsThread::callback() {
    if (actors.empty()) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
        if (endTick) {
            endTick = false;
            renderThread.setEnd();
        }
    } else {
        mutex.lock();
        const Actor* actor = actors.top();
        actors.pop();
        mutex.unlock();
        renderThread.pushComponents(actor->getActorComponents());
    }
}

void ComponentsThread::pushActor(const Actor* actor) {
    std::lock_guard<std::mutex> lock(mutex);
    actors.push(actor);
}

bool ComponentsThread::empty() {
    std::lock_guard<std::mutex> lock(mutex);
    return actors.empty() && renderThread.empty();
}

void ComponentsThread::setEnd()
{
    endTick = true;
}


SceneThread::SceneThread()
    : componentsThread(&endTick)
{};

void SceneThread::start() {
    std::thread(&SceneThread::job, this).detach();
    std::thread(&ComponentsThread::job, &componentsThread).detach();
    componentsThread.renderThread.job();
}
void SceneThread::setScene(const Scene* _scene)
{
    scene = _scene;
}

void SceneThread::job() {
    while (!GameManager::IsEnd) {
        callback();
    }
}


bool isVisible(Camera *camera, Actor *object)
{
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

void SceneThread::callback()
{
    static const int targetFPS = 400;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(frameStart + frameDuration);
    frameStart = std::chrono::steady_clock::now();
    GameManager::Time.Update();
    Actor::PushTime(GameManager::Time.GetCurrentTime());

    static time_t prev = time(0);
    static int frame = 0;

    // for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        // GameManager::render->drawObject(&it->transform, it->sprite);

    for (auto& it : scene->actors) {
        if (*it->GetName() == "Wilson") {
             (reinterpret_cast<Pawn*>(it))->MoveForward();
            it->updateAnimation(it->GetAnimation(GameManager::Time.GetCurrentTime()));
        } else if (*it->GetName() == "Spider") {
            (reinterpret_cast<Pawn*>(it))->MoveTowards(scene->actors[0], 0.006);
        }
        // if (isVisible(GameManager::callbackData.camera, it)) {
            componentsThread.pushActor(it);
        // }
    }

    componentsThread.setEnd();
    while (!endTick) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
    }
    endTick = false;

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        // //std::cout << "Total time: " << totalTime.count() << " milliseconds" << std::endl;
        // totalTime = std::chrono::milliseconds(0);
        prev = time(0);
        frame = 0;
    }
}
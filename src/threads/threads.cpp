#include <threads/threads.hpp>
#include <game/gameManager.hpp> 


RenderThread::RenderThread(std::atomic<bool>* endTickPtr)
    : sceneEndTickPtr(endTickPtr)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::job() {
    while (!GameManager::IsEnd) callback();

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(200)));
    std::cout << "RenderThread worked for: " << workDuration.count() << " | " << idleDuration.count() << " | " << swapDuration.count() << " seconds\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(50)));
}

void RenderThread::callback() {
    static std::size_t index = 0;
    if (empty()) {
        startWorkTime = std::chrono::high_resolution_clock::now();
        if (endTicks == false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_RENDER));
            endWorkTime = std::chrono::high_resolution_clock::now();
            idleDuration += endWorkTime - startWorkTime;
            return;
        }
        endTicks = false;
        *sceneEndTickPtr = true;
        endWorkTime = std::chrono::high_resolution_clock::now();
        idleDuration += endWorkTime - startWorkTime;

        startWorkTime = std::chrono::high_resolution_clock::now();
        swapBuffer();
        endWorkTime = std::chrono::high_resolution_clock::now();
        swapDuration += endWorkTime - startWorkTime;
    } else {
        startWorkTime = std::chrono::high_resolution_clock::now();
        mutex.lock();
        std::pair<Matrix4f<GLfloat>, Sprite*> sprite = sprites.top();
        sprites.pop();
        mutex.unlock();
        GameManager::render->drawObject(sprite.first, sprite.second);

        endWorkTime = std::chrono::high_resolution_clock::now();
        workDuration += endWorkTime - startWorkTime;
    }
}

void RenderThread::swapBuffer()
{
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::pushSprite(const std::pair<Matrix4f<GLfloat>, Sprite*>& componentSprite) {
    std::lock_guard<std::mutex> lock(mutex);
    sprites.push(componentSprite);
}

bool RenderThread::empty() {
    return sprites.empty();
}

void RenderThread::setEnd()
{
    endTicks = true;
}


ComponentsThread::ComponentsThread(std::atomic<bool>* endTickPtr)
    : renderThread(endTickPtr) 
{}

void ComponentsThread::job() {
    while (!GameManager::IsEnd) callback();
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(50)));

    std::cout << "ComponentsThread worked for: " << workDuration.count() << " | " << idleDuration.count() << " seconds\n";
}

void ComponentsThread::callback() {
    if (actors.empty()) {
        if (endTick) {
            endTick = false;
            renderThread.setEnd();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_COMPONENTS));
        }
    } else {
        startWorkTime = std::chrono::high_resolution_clock::now();

        mutex.lock();
        Actor* actor = actors.top();
        actors.pop();
        mutex.unlock();
        for (auto& it : actor->getActorComponents()) {
            renderThread.pushSprite(
                std::pair<Matrix4f<GLfloat>, Sprite *>(
                    GameManager::render->pipeline.GetTransform(it->transform), it->sprite
                )
            );
        }

        endWorkTime = std::chrono::high_resolution_clock::now();
        workDuration += endWorkTime - startWorkTime;
    }
}

void ComponentsThread::pushActor(Actor* actor) {
    std::lock_guard<std::mutex> lock(mutex);
        startWorkTime = std::chrono::high_resolution_clock::now();
    actors.push(actor);
        endWorkTime = std::chrono::high_resolution_clock::now();
        idleDuration += endWorkTime - startWorkTime;
}

bool ComponentsThread::empty() {
    return actors.empty();
}

void ComponentsThread::setEnd()
{
    endTick = true;
}


SceneThread::SceneThread()
    : componentsThread(&endTick)
{}

void SceneThread::start() {
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    std::thread(&SceneThread::job, this).detach();
    std::thread(&ComponentsThread::job, &componentsThread).detach();
    componentsThread.renderThread.job();
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Total = endTime - startTime;
    std::cout << "Total worked for: " << Total.count() << " seconds\n";
    // job();
}

void SceneThread::setScene(const Scene* _scene)
{
    scene = _scene;
}

void SceneThread::job() {
    while (!GameManager::IsEnd) {
        callback();
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100)));

    std::cout << "SceneThread worked for: " << workDuration.count() << " seconds\n";

    // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(400)));
    // std::cout << "RenderThread worked for: " << renderThread.workDuration.count() << " seconds\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(200)));

}


bool isVisible(Camera *camera, Actor *object)
{
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

void SceneThread::callback()
{
    startWorkTime = std::chrono::high_resolution_clock::now();


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
    static std::size_t index = 0; 
    static std::size_t sleep = THREADS_SLEEP_TIME_MS; 


    // for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        // GameManager::render->drawObject(&it->transform, it->sprite);

    for (auto& it : scene->actors) {
        if (*it->GetName() == "Wilson") {
             (reinterpret_cast<Pawn*>(it))->MoveForward();
            it->updateAnimation(it->GetAnimation(GameManager::Time.GetCurrentTime()));
        } else if (*it->GetName() == "Spider") {
            (reinterpret_cast<Pawn*>(it))->MoveTowards(scene->actors[1], 0.008);
        }

        // if (isVisible(GameManager::callbackData.camera, it)) {
            componentsThread.pushActor(it);
        // }
    }

    componentsThread.setEnd();

    endWorkTime = std::chrono::high_resolution_clock::now();
    workDuration += endWorkTime - startWorkTime;

    while (!endTick) {
        if (GameManager::IsEnd) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_SCENE));
        // renderThread.callback();
    }

    endTick = false;

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        sleep = 1000 / frame;
        prev = time(0);
        frame = 0;
    }
}
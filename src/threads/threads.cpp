#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.3, 0.7);


RenderThread::RenderThread(std::atomic<bool>* endTickPtr, std::size_t numComponentsThreads)
    : sceneEndTickPtr(endTickPtr), num(numComponentsThreads)
{
    // endTicks = new std::atomic<bool>[num];
    for (std::size_t i = 0; i < num; i++) {
        endTicks[i] = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::job() {
    while (!GameManager::IsEnd) callback();

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(400)));
    std::cout << "RenderThread worked for: " << workDuration.count() << " | " << idleDuration.count() << " | " << swapDuration.count() << " seconds\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(200)));
}

void RenderThread::callback() {
    static std::size_t index = 0;
    if (empty()) {
        startWorkTime = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < num; i++) {
            if (endTicks[i] == false) {
                std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
                endWorkTime = std::chrono::high_resolution_clock::now();
                idleDuration += endWorkTime - startWorkTime;
                return;
            }
        }
        for (std::size_t i = 0; i < num; i++) {
            endTicks[i] = false;
        }
        *sceneEndTickPtr = true;
        endWorkTime = std::chrono::high_resolution_clock::now();
        idleDuration += endWorkTime - startWorkTime;

        startWorkTime = std::chrono::high_resolution_clock::now();
        swapBuffer();
        endWorkTime = std::chrono::high_resolution_clock::now();
        swapDuration += endWorkTime - startWorkTime;
    } else {
        startWorkTime = std::chrono::high_resolution_clock::now();
        index = (index + 1) % num;
        if (sprites[index].empty()) return;
        mutex[index].lock();
        std::pair<Matrix4f<GLfloat>, Sprite*> sprite = sprites[index].top();
        sprites[index].pop();
        mutex[index].unlock();
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

void RenderThread::pushSprite(const std::pair<Matrix4f<GLfloat>, Sprite*>& componentSprite, std::size_t n) {
    std::lock_guard<std::mutex> lock(mutex[n]);
    // std::cout << "pushSprite " << sprites[n].size() << std::endl;
    sprites[n].push(componentSprite);
}

bool RenderThread::empty() {
    for (std::size_t i = 0; i < num; i++) {
        // std::lock_guard<std::mutex> lock(mutex[i]);
        if (!sprites[i].empty()) return false;
    }
    return true;
}

void RenderThread::setEnd(std::size_t n)
{
    endTicks[n] = true;
}

// Реализация методов ComponentsThread

ComponentsThread::ComponentsThread() {}

void ComponentsThread::init(std::size_t n, RenderThread* renderThreadPtr)
{
    num = n;
    renderThread = renderThreadPtr;
}

void ComponentsThread::job() {
    while (!GameManager::IsEnd) callback();
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100 * (num + 1))));

    std::cout << "ComponentsThread worked for: " << workDuration.count() << " | " << idleDuration.count() << " seconds\n";
}

void ComponentsThread::callback() {
    if (actors.empty()) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
        if (endTick) {
            endTick = false;
            renderThread->setEnd(num);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS));
        }
    } else {
        startWorkTime = std::chrono::high_resolution_clock::now();

        mutex.lock();
        const Actor* actor = actors.top();
        actors.pop();
        mutex.unlock();
        for (const auto& it : actor->getActorComponents()) {
            renderThread->pushSprite(
                std::pair<Matrix4f<GLfloat>, Sprite *>(
                    GameManager::render->pipeline.GetTransform(it->transform), it->sprite
                ),
                num
            );
        }

        endWorkTime = std::chrono::high_resolution_clock::now();
        workDuration += endWorkTime - startWorkTime;
    }
}

void ComponentsThread::pushActor(const Actor* actor) {
    std::lock_guard<std::mutex> lock(mutex);
        startWorkTime = std::chrono::high_resolution_clock::now();
    actors.push(actor);
        endWorkTime = std::chrono::high_resolution_clock::now();
        idleDuration += endWorkTime - startWorkTime;
}

bool ComponentsThread::empty() {
    // std::lock_guard<std::mutex> lock(mutex);
    return actors.empty();
}

void ComponentsThread::setEnd()
{
    endTick = true;
}


SceneThread::SceneThread(std::size_t numComponentsThreads)
    : numThreads(numComponentsThreads), renderThread(&endTick, numComponentsThreads) 
{
    // componentsThreads = new ComponentsThread[numThreads]();
    for (std::size_t i = 0; i < numThreads; i++) {
        componentsThreads[i].init(i, &renderThread);
    }
};

SceneThread::SceneThread()
    : SceneThread(1)
{};

void SceneThread::start() {
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    std::thread(&SceneThread::job, this).detach();
    for (std::size_t i = 0; i < numThreads; i++) {
        std::thread(&ComponentsThread::job, &componentsThreads[i]).detach();
    }
    renderThread.job();
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
            (reinterpret_cast<Pawn*>(it))->MoveTowards(scene->actors[0], 0.006);
        }
        // if (isVisible(GameManager::callbackData.camera, it)) {
            index = (index + 1) % numThreads;
            componentsThreads[index].pushActor(it);
        // }
    }

    for (std::size_t i = 0; i < numThreads; i++) {
        componentsThreads[i].setEnd();
    }

    endWorkTime = std::chrono::high_resolution_clock::now();
    workDuration += endWorkTime - startWorkTime;

    while (!endTick) {
    // startWorkTime = std::chrono::high_resolution_clock::now();
        if (GameManager::IsEnd) return;
    // endWorkTime = std::chrono::high_resolution_clock::now();
    // idleDuration += endWorkTime - startWorkTime;
        std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * 10));
        // renderThread.callback();
    }

    endTick = false;

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        sleep = 1000 / frame;
        // //std::cout << "Total time: " << totalTime.count() << " milliseconds" << std::endl;
        // totalTime = std::chrono::milliseconds(0);
        prev = time(0);
        frame = 0;
    }
}
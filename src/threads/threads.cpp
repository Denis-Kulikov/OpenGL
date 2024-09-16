#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 


RenderThread::RenderThread(std::atomic<bool>* endTickPtr)
    : sceneEndTickPtr(endTickPtr)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::job() {
    while (!GameManager::IsEnd) callback();
}

void RenderThread::callback() {
    static Cube cube("img/skybox.png");
    static Mesh model1(std::string("assets/model/2b/2b.fbx"));
    // static Mesh model2(std::string("assets/model/ramen/source/ramen shop/ramen shop/ShoWaHouse01.FBX"));
    // static Mesh model3(std::string("assets/model/sidewalk/source/SIDEWALK.fbx"));
    // static Mesh model4(std::string("assets/model/2b.fbx"));
    objectTransform transform;
    transform.SetWorldPos(0.0, -10.0, -10.0);
    transform.SetRotate(-90.0, 0.0, 0.0);
    float scale = 5;
    transform.MultiplyScale(Vector3<GLfloat>(scale, scale, scale));

    objectTransform transform_ramen;
    transform_ramen.SetWorldPos(-60.0, 15.9, 25.0);
    transform_ramen.SetRotate(-90.0, -90.0, 0.0);
    transform_ramen.MultiplyScale(Vector3<GLfloat>(6.1, 6.1, 6.1));

    objectTransform transform_road;
    transform_road.SetWorldPos(-20.0, -11.0, -45.0);
    transform_road.SetRotate(0.0, -90.0, 0.0);
    transform_road.MultiplyScale(Vector3<GLfloat>(3.1, 3.1, 3.1));

    if (sprites.empty()) {
        if (endTicks == false) {
            endWorkTime = std::chrono::high_resolution_clock::now();
            idleDuration += endWorkTime - startWorkTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_RENDER * 10));
            return;
        }

        GameManager::render->clearRender();
        model1.Render((transform));
        // model2.Render(GameManager::render->pipeline.GetTransform(transform_ramen));
        // model3.Render(GameManager::render->pipeline.GetTransform(transform_road));
        GameManager::render->clearRender();

        endTicks = false;
        *sceneEndTickPtr = true;

        startWorkTime = std::chrono::high_resolution_clock::now();
        swapBuffer();
        GameManager::render->drawSkybox(cube);
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

void RenderThread::swapBuffer() {
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::pushSprite(const std::pair<Matrix4f<GLfloat>, Sprite*>& componentSprite) {
    std::lock_guard<std::mutex> lock(mutex);
    sprites.push(componentSprite);
}

void RenderThread::setEnd() {
    endTicks = true;
}


ComponentsThread::ComponentsThread(std::atomic<bool>* endTickPtr)
    : renderThread(endTickPtr) 
{}

void ComponentsThread::job() {
    while (!GameManager::IsEnd) callback();
}

void ComponentsThread::callback() {
    if (actors.empty()) {
        if (endTick) {
            
            endTick = false;
            renderThread.setEnd();
        } else {
            // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_COMPONENTS));
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

void ComponentsThread::setEnd() {
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
}

void SceneThread::setScene(const Scene* _scene) {
    scene = _scene;
}

void SceneThread::job() {
    while (!GameManager::IsEnd) callback();
}


bool isVisible(Camera *camera, Actor *object) {
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

void SceneThread::callback() {
    startWorkTime = std::chrono::high_resolution_clock::now();

    static const int targetFPS = 144;
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

    GameManager::UpdateCamera();

    for (auto& it : scene->actors) {
        (reinterpret_cast<Pawn*>(it))->MoveForward();
        it->updateAnimation(it->GetAnimation(GameManager::Time.GetCurrentTime()));

        componentsThread.pushActor(it);
    }

    componentsThread.setEnd();

    endWorkTime = std::chrono::high_resolution_clock::now();
    workDuration += endWorkTime - startWorkTime;

    startWorkTime = std::chrono::high_resolution_clock::now();

    while (!endTick) {
        if (GameManager::IsEnd) return;
    }
    endWorkTime = std::chrono::high_resolution_clock::now();
    idleDuration += endWorkTime - startWorkTime;

    endTick = false;

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    }
}
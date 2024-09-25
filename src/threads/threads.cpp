#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 

#include <entities/templates/mobs/Female.hpp>

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
    static Sprite sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");
    static Female female;
    objectTransform transform;

    float scale = 5;
    static bool flag = true;
    if (flag) {
        transform.SetWorldPos(0.0, -10.0, -10.0);
        transform.SetRotate(0.0, 0.0, 0.0);
        transform.MultiplyScale(glm::vec3(scale, scale, scale));

        female.GetMesh()->set_transform(transform);
        
        transform.MultiplyScale(glm::vec3(1/scale, 1/scale, 1/scale));
        flag = !flag;
    }

    scale = 10;
    transform.SetWorldPos(0.0, -10.0, -10.0);
    transform.SetRotate(90.0, 0.0, 0.0);
    transform.MultiplyScale(glm::vec3(scale, scale, scale));


    if (sprites.empty()) {
        if (endTicks == false) {
            endWorkTime = std::chrono::high_resolution_clock::now();
            idleDuration += endWorkTime - startWorkTime;
            // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_RENDER * 1));
            return;
        }

        std::vector<aiMatrix4x4> *Transforms = new std::vector<aiMatrix4x4>;
        female.GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), *Transforms);
        GameManager::render->clearRender();
        Actor::Actor_rdata data = {Transforms, female.GetMesh()};
        female.Render(&data);
        GameManager::render->clearRender();

        // sprite.Render(&GameManager::render->pipeline.GetTransform(transform));

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
        std::pair<Matrix4f, Sprite*> sprite = sprites.top();
        sprites.pop();
        mutex.unlock();

        endWorkTime = std::chrono::high_resolution_clock::now();
        workDuration += endWorkTime - startWorkTime;
    }
}

void RenderThread::swapBuffer() {
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderThread::pushSprite(const std::pair<Matrix4f, Sprite*>& componentSprite) {
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
            // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_COMPONENTS * 20));
        }
    } else {
        startWorkTime = std::chrono::high_resolution_clock::now();

        mutex.lock();
        Actor* actor = actors.top();
        actors.pop();
        mutex.unlock();
        //for (auto& it : actor->getActorComponents()) {
        //    renderThread.pushSprite(
        //        std::pair<Matrix4f, Sprite *>(
        //            GameManager::render->pipeline.GetTransform(it->transform), it->sprite
        //        )
        //    );
        //}

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


// bool isVisible(Camera *camera, Actor *object) {
//     glm::vec3 cameraDirection = gml::normalize(camera->Params.Target);
//     glm::vec3 cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();
//     return cameraDirection.VDot(cameraToObject) > 0;
// }

void SceneThread::callback() {
    startWorkTime = std::chrono::high_resolution_clock::now();

    static const int targetFPS = 144;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(frameStart + frameDuration);
    frameStart = std::chrono::steady_clock::now();
    GameManager::Time.Update();
    GameManager::render->GetPV();

    static time_t prev = time(0);
    static int frame = 0;
    static std::size_t index = 0; 

    for (auto& it : scene->actors) {
        (reinterpret_cast<Pawn*>(it))->MoveForward();
        // componentsThread.pushActor(it);
    }
    GameManager::UpdateCamera();

    componentsThread.setEnd();

    endWorkTime = std::chrono::high_resolution_clock::now();
    workDuration += endWorkTime - startWorkTime;

    startWorkTime = std::chrono::high_resolution_clock::now();

    while (!endTick) {
        if (GameManager::IsEnd) return;
            // std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_RENDER * 1));
    }
    endWorkTime = std::chrono::high_resolution_clock::now();
    idleDuration += endWorkTime - startWorkTime;

    endTick = false;
    [](){ return true; };

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    }
}
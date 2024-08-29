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
    std::cout << "RenderThread worked for: " << workDuration.count() << " | " << swapDuration.count() << " seconds\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(50)));
}

void RenderThread::callback() {
    if (sprites.empty()) {
        if (endTicks == false) {
            endWorkTime = std::chrono::high_resolution_clock::now();
            idleDuration += endWorkTime - startWorkTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(THREADS_SLEEP_TIME_MS * THREAD_RENDER));
            return;
        }
        endTicks = false;
        *sceneEndTickPtr = true;

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
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(50)));

    std::cout << "ComponentsThread worked for: " << workDuration.count() << " | " << idleDuration.count() << " seconds\n";
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

    std::thread t_sceneThread(&SceneThread::job, this);
    std::thread t_componentsThread(&ComponentsThread::job, &this->componentsThread);
    
    t_sceneThread.detach();
    t_componentsThread.detach();

    renderThread = &componentsThread.renderThread;
    renderThread->job();

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Total = endTime - startTime;
    std::cout << "Total worked for: " << Total.count() << " seconds\n";
}


void SceneThread::setScene(const Scene* _scene) {
    scene = _scene;
}

void SceneThread::job() {
    while (!GameManager::IsEnd) callback();
    std::cout << "SceneThread worked for: " << workDuration.count() << " seconds\n";
}


bool isVisible(Camera *camera, Actor *object) {
    Vector3<GLfloat> cameraDirection = camera->Params.Target.Normalize();
    Vector3<GLfloat> cameraToObject = (object->GetTransform()->GetWorldPos() - camera->Params.WorldPos).Normalize();

    return cameraDirection.VDot(cameraToObject) > 0;
}

void createSymbols(std::vector<Sprite> &symbols)
{
    for (int i = 32; i < 128; i++) {
        std::string textureName(1, i);
        symbols[i - 32].SetTexture(textureName);
    }
}

void SceneThread::drowNode(objectTransform transform, const std::string &text)
{
    static std::vector<Sprite> symbols(128 - 32, Sprite(std::string("Symbol"), "shaders/symbol_fs.glsl", "shaders/sprite_vs.glsl", "a", Vector3<GLfloat>(0.0, 0.0, 0.0)));
    static Sprite mySprite(std::string("Node"), "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", "img/cell.jpg");
    
    const GLfloat frame = 1.1;

    static bool b = true;

    if (b) {
        createSymbols(symbols);
        b = !b;
    } 

    transform.Scale.x *= text.size() * frame;
    transform.Scale.z = transform.Scale.y *= frame;
    renderThread->pushSprite(
        std::pair<Matrix4f<GLfloat>, Sprite *>(
            GameManager::render->pipeline.GetTransform(transform), &mySprite
        )
    );
    transform.Scale.x /= text.size() * frame;
    transform.Scale.z = transform.Scale.y /= frame;

    const GLfloat save_width = transform.Scale.x;
    const GLfloat save_x = transform.WorldPos.x;
    const GLfloat offset = transform.Scale.x * frame * 2;
    transform.WorldPos.x += ((text.size() - 1) * offset) / 2;
    transform.WorldPos.z += 0.01;

    for (const auto &it : text) {
        if (it < 32 || it > 127) continue;

        renderThread->pushSprite(
            std::pair<Matrix4f<GLfloat>, Sprite *>(
                GameManager::render->pipeline.GetTransform(transform), &symbols[it - 32]
            )
        );

        transform.WorldPos.x -= offset;
    }

    transform.WorldPos.z -= 0.01;
    transform.WorldPos.x = save_x;
}

void SceneThread::drowBinaryTree(const objectTransform &transform, const tree_node<std::string> &node)
{
    drowNode(transform, node.get_value());

    objectTransform left(transform); 
    objectTransform right(transform);
    
    right.WorldPos.y = left.WorldPos.y -= left.GetScale().x * 2;

    left.SetScale(left.GetScale().x / 2);
    right.SetScale(right.GetScale().x / 2);


    left.WorldPos.x += 3 * transform.GetScale().x;
    right.WorldPos.x -= 3 * transform.GetScale().x;

    if (node.left_  != nullptr) drowBinaryTree(left, node.left());
    if (node.right_ != nullptr) drowBinaryTree(right, node.right());
}

tree_node<std::string>* deserialize(std::ifstream& in) {
    std::string value;
    in >> value;
    if (value == "#") return nullptr;

    tree_node<std::string>* node = new tree_node<std::string>(value);
    node->left_ = deserialize(in);
    node->right_ = deserialize(in);
    return node;
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

    // 
    static_cast<Pawn*>(scene->actors[0])->MoveForward();

    const std::size_t size = 100;
    objectTransform transform;
    transform.SetRotate(0, 0, 180);

    std::ifstream in("tree.txt");
    drowBinaryTree(transform, *deserialize(in));
    //

    renderThread->setEnd();

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
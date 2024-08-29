#include <game/gameManager.hpp> 

// #include <fcntl.h>
// #include <unistd.h>

GameManager::GameManager() {};

GameManager::~GameManager()
{
    delete render;
    delete threads;
    delete callbackData.camera;
}

void GameManager::Intit_characters()
{
    FT_Face face;

    if (FT_New_Face(ft, "assets/ttf/ArialRegular.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters->insert(std::pair<GLchar, Character>(c, character));

        std::string s(1, c); 
        Sprite::PushTexture(texture, std::string(s));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
}

void GameManager::PushCamera(Camera *_camera)
{
    callbackData.camera = _camera;
}

void GameManager::PushPlayer(Pawn *_player)
{
    callbackData.player = _player;
}

Camera *GameManager::createCamera()
{
    auto camera = new Camera();
    if (camera == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the camera" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vector3<GLfloat> CameraPos(0.0f, 0.2f, 10);
    Vector3<GLfloat> CameraTarget(0.0f, 0.0f, -1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    return camera;
}

void GameManager::KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keys[GLFW_KEY_LAST] = {false};

    const GLfloat speed_rotation = -0.125;
    const GLfloat speed_movement = 0.1;

    Pawn* player = callbackData.player;
    Camera* camera = callbackData.camera;

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if (keys[GLFW_KEY_F]) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        GameManager::IsEnd = true;
    }

    player->SetDirection(Vector3<GLfloat>(
        keys[GLFW_KEY_A] - keys[GLFW_KEY_D], 
        keys[GLFW_KEY_W] - keys[GLFW_KEY_S], 
        keys[GLFW_KEY_LEFT_SHIFT] - keys[GLFW_KEY_SPACE]
    ));

    if (keys[GLFW_KEY_E]) camera->Params.Target.x += speed_rotation;
    if (keys[GLFW_KEY_Q]) camera->Params.Target.x -= speed_rotation;

    if (keys[GLFW_KEY_EQUAL]) player->SetSpeed(player->GetSpeed() * 1.4);
    if (keys[GLFW_KEY_MINUS]) player->SetSpeed(player->GetSpeed() / 1.4);

    if (keys[GLFW_KEY_ENTER]) {
        player->GetTransform()->SetWorldPos(Vector3<GLfloat>(0, -1, 0));
        player->SetSpeed(0.5);
    }
}

void GameManager::InitializeObjects()
{
    Sprite::initializeGeometry();
    // line::initializeGeometry();
}

void GameManager::InitializeGLFW(int _width, int _height)
{
    if (!glfwInit()) {
        std::cerr << "Error: " << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = _width;
    height = _height;

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Error: " << "Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, GameManager::KeyboardCB);
    glfwSetWindowUserPointer(window, &callbackData);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << "Failed to initialize glew" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    PushCamera(createCamera());

    render = new Render(callbackData.camera);
    if (render == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the renderer" << std::endl;
        exit(EXIT_FAILURE);
    }

    threads = new SceneThread();
    if (threads == nullptr) {
        std::cerr << "Error: " << "Failed to allocate memory to the threads" << std::endl;
        exit(EXIT_FAILURE);
    }


    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        return;
    }
    Characters = new std::map<GLchar, Character>;

    Intit_characters();
}
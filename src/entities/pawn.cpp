#include <entities/pawn.hpp>

Pawn::Pawn(const std::string &path, const size_t SkeletSize)
    : Actor(path, SkeletSize),
        camera(nullptr)
{}

Pawn::~Pawn()
{
    if (camera != nullptr) delete camera;
}

void Pawn::Teleport(const Vector3<GLfloat> newPosition) {
    this->trans.WorldPos = newPosition;
}

void Pawn::Move(const Vector3<GLfloat> offset) {
    this->trans.Move(offset);
}

void Pawn::Move(const GLfloat distance, const Vector3<GLfloat> direction) {
    this->trans.Move(distance, direction);
}

void Pawn::MoveForward() {
    this->trans.Move(this->speed, this->direction);
}

void Pawn::MoveForward(const GLfloat distance) {
    this->trans.Move(distance, this->direction);
}

// template<typename T> // ***
// void Pawn::MoveTowards(T* targetPawn, GLfloat speed) {
//     Vector3<GLfloat> direction = targetPawn->GetTransform()->WorldPos - this->trans.WorldPos;
//     Move(speed, direction);
// }

void Pawn::Rotate(const Vector3<GLfloat> _rotate) {
    this->trans.Rotate = _rotate;
}

void Pawn::AddRotate(const Vector3<GLfloat> _rotate) {
    this->trans.Rotate += _rotate;
}

void Pawn::SetScale(const Vector3<GLfloat> _scale) {
    this->trans.Scale = _scale;
}

void Pawn::MultiplyScale(const Vector3<GLfloat> _scale) {
    this->trans.Scale *= _scale;
}

void Pawn::attachCamera(Camera *_camera)
{
    camera = _camera;
}

bool Pawn::createCamera(int width, int height)
{
    auto _camera = new Camera();

    Vector3<GLfloat> CameraPos(0.0f, 3.0f, 15);
    Vector3<GLfloat> CameraTarget(0.0f, -0.4f, -1.0f);
    Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

    _camera->SetCamera(CameraPos, CameraTarget, CameraUp);
    _camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

    attachCamera(_camera);
    _camera->attachOwnerTransform(this->GetTransform());

    return true;
}

Camera *Pawn::GetCamera()
{
    return camera;
}

Vector3<GLfloat> Pawn::GetCameraPosition()
{
    return camera->GetPosition();
}

GLfloat Pawn::GetSpeed()
{
    return speed;
}

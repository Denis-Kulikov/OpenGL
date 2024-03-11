#include <entities/pawn.hpp>

Pawn::Pawn(const std::string &path) : Actor(path) 
{
    cameraPos = Vector3<GLfloat>(0.0f, 6.0f, 15);
}

void Pawn::Teleport(const Vector3<GLfloat> newPosition) {
    trans.WorldPos = newPosition;
}

void Pawn::Move(const Vector3<GLfloat> offset) {
    trans.Move(offset);
}

void Pawn::Move(const GLfloat distance, const Vector3<GLfloat> direction) {
    trans.Move(distance, direction);
}

void Pawn::MoveForward(const GLfloat distance) {
    trans.MoveForward(distance);
}

void Pawn::Rotate(const Vector3<GLfloat> _rotate) {
    trans.Rotate = _rotate;
}

void Pawn::AddRotate(const Vector3<GLfloat> _rotate) {
    trans.Rotate += _rotate;
}

void Pawn::SetScale(const Vector3<GLfloat> _scale) {
    trans.Scale = _scale;
}

void Pawn::MultiplyScale(const Vector3<GLfloat> _scale) {
    trans.Scale *= _scale;
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

    return true;
}


void Pawn::MoveTowards(Pawn* targetPawn, GLfloat speed) {
    // Получаем позиции текущей пешки и целевой пешки
    Vector3<GLfloat> currentPosition = trans.WorldPos;
    Vector3<GLfloat> targetPosition = targetPawn->trans.WorldPos;

    // Вычисляем вектор направления от текущей позиции к целевой позиции
    Vector3<GLfloat> direction = targetPosition - currentPosition;
    direction.Normalize(); // Нормализуем вектор направления

    // Вычисляем вектор перемещения, умножая вектор направления на скорость
    Vector3<GLfloat> offset = direction * speed;

    // Двигаем текущую пешку по вычисленному вектору перемещения
    Move(offset);
}


void Pawn::UpdateCameraPos()
{
    camera->Params.WorldPos = cameraPos + trans.WorldPos;
}

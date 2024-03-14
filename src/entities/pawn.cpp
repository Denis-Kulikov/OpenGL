#include <entities/pawn.hpp>



// template <typename Derived>
// void Pawn<Derived>::Teleport(const Vector3<GLfloat> newPosition) {
//     this->trans.WorldPos = newPosition;
// }

// template <typename Derived>
// void Pawn<Derived>::Move(const Vector3<GLfloat> offset) {
//     this->trans.Move(offset);
// }

// template <typename Derived>
// void Pawn<Derived>::Move(const GLfloat distance, const Vector3<GLfloat> direction) {
//     this->trans.Move(distance, direction);
// }

// template <typename Derived>
// void Pawn<Derived>::MoveForward(const GLfloat distance) {
//     this->trans.MoveForward(distance);
// }

// template <typename Derived>
// void Pawn<Derived>::Rotate(const Vector3<GLfloat> _rotate) {
//     this->trans.Rotate = _rotate;
// }

// template <typename Derived>
// void Pawn<Derived>::AddRotate(const Vector3<GLfloat> _rotate) {
//     this->trans.Rotate += _rotate;
// }

// template <typename Derived>
// void Pawn<Derived>::SetScale(const Vector3<GLfloat> _scale) {
//     this->trans.Scale = _scale;
// }

// template <typename Derived>
// void Pawn<Derived>::MultiplyScale(const Vector3<GLfloat> _scale) {
//     this->trans.Scale *= _scale;
// }


// template <typename Derived>
// void Pawn<Derived>::attachCamera(Camera *_camera)
// {
//     camera = _camera;
// }

// template <typename Derived>
// bool Pawn<Derived>::createCamera(int width, int height)
// {
//     auto _camera = new Camera();

//     Vector3<GLfloat> CameraPos(0.0f, 3.0f, 15);
//     Vector3<GLfloat> CameraTarget(0.0f, -0.4f, -1.0f);
//     Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

//     _camera->SetCamera(CameraPos, CameraTarget, CameraUp);
//     _camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

//     attachCamera(_camera);

//     return true;
// }


// template <typename Derived>
// void Pawn<Derived>::MoveTowards(Pawn* targetPawn, GLfloat speed) {
//     // Получаем позиции текущей пешки и целевой пешки
//     Vector3<GLfloat> currentPosition = this->trans.WorldPos;
//     Vector3<GLfloat> targetPosition = targetPawn->trans.WorldPos;

//     // Вычисляем вектор направления от текущей позиции к целевой позиции
//     Vector3<GLfloat> direction = targetPosition - currentPosition;
//     direction.Normalize(); // Нормализуем вектор направления

//     // Вычисляем вектор перемещения, умножая вектор направления на скорость
//     Vector3<GLfloat> offset = direction * speed;

//     // Двигаем текущую пешку по вычисленному вектору перемещения
//     Move(offset);
// }


// template <typename Derived>
// void Pawn<Derived>::UpdateCameraPos()
// {
//     this->camera->Params.WorldPos = cameraPos + this->trans.WorldPos;
// }

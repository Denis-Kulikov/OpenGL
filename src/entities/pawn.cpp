#include <entities/pawn.hpp>

Pawn::Pawn(const std::string &path, const size_t SkeletSize)
    : Actor(path, SkeletSize)
{}

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

GLfloat Pawn::GetSpeed()
{
    return speed;
}

void Pawn::SetSpeed(GLfloat s)
{
    speed = s;
}

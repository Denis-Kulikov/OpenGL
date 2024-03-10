#include <entities/pawn.hpp>

Pawn::Pawn(const std::string &path) : Actor(path) {}

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

// #include <object/component/component.hpp>
// #include <object/transform/transform.hpp>
// #include <object/transform/rigid_transform.hpp>

// // ================================================================= //
// // ##############| Явная специализация для Transform |#################
// // ================================================================= //
// template <>
// ComponentTransform<Transform>::ComponentTransform(Transform* globalTransform_) {
//     globalTransform = globalTransform_;
// }

// template <>
// Transformable* ComponentTransform<Transform>::GetGlobalTransform() {
//     return globalTransform;
// }

// template <>
// const Transformable* ComponentTransform<Transform>::GetGlobalTransform() const {
//     return globalTransform;
// }

// template <>
// void ComponentTransform<Transform>::SetGlobalTransform(Transformable* newTransform) {
//     globalTransform = dynamic_cast<Transform*>(newTransform);
// }









// // ================================================================= //
// // ##############| Явная специализация для RigidTransfor |#############
// // ================================================================= //
// template <>
// ComponentTransform<RigidTransform>::ComponentTransform(RigidTransform* globalTransform_) {
//     globalTransform = globalTransform_;
// }

// template <>
// Transformable* ComponentTransform<RigidTransform>::GetGlobalTransform() {
//     return globalTransform;
// }

// template <>
// const Transformable* ComponentTransform<RigidTransform>::GetGlobalTransform() const {
//     return globalTransform;
// }

// template <>
// void ComponentTransform<RigidTransform>::SetGlobalTransform(Transformable* newTransform) {
//     globalTransform = dynamic_cast<RigidTransform*>(newTransform);
// }

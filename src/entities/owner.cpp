#include <entities/owner.hpp>
#include <game/gameManager.hpp>

//Owner::Owner() {};
//
//void Owner::UpdateCamera()
//{
//    const float yaw_speed = 90.0;
//        yaw += yaw_speed * player->GetTarget().x * GameManager::Time.GetDeltaTime();
//        camera->Params.Target.x = -cos(ToRadian(yaw));
//        camera->Params.Target.z = -sin(ToRadian(yaw));
//        
//    const float pitch_speed = 90.0;
//    const float pitch_limit = 90.0;
//        pitch += pitch_speed * player->GetTarget().x * GameManager::Time.GetDeltaTime();
//        if (fabs(pitch) > pitch_limit) pitch = std::copysign(pitch_limit - 1e-9, pitch);
//        camera->Params.Target.y = tan(ToRadian(pitch));
//        camera->PersProj.FOV = pitch_limit - fabs(pitch);
//        std::cout << "FOV: " << camera->PersProj.FOV << std::endl;
//}
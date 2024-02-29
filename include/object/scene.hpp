#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"

class Scene {
public:
    Scene(std::string _name) : name(_name) {};
    ~Scene() {};

    void pushObject(sprite* _sprite) { sprites.push_back(_sprite); }
    // void pushObject(sphere* _sphere) { spheres.push_back(_sphere); }
    // void pushObject(circle* _circle) { circles.push_back(_circle); }
    // void pushObject(line* _line) { lines.push_back(_line); }
    // void pushObject(cube_bone* _cube_bone) { cube_bones.push_back(_cube_bone); }

    void deleteObject(sprite* _sprite) {}
    // void deleteObject(sphere* _sphere) {}
    // void deleteObject(circle* _circle) {}
    // void deleteObject(line* _line) {}
    // void deleteObject(cube_bone* _cube_bone) {}

    std::vector<sprite*>::iterator getIterator() { return sprites.begin(); }
    // std::vector<sprite*>::iterator getIteratorSprite() { return sprites.begin(); }
    // std::vector<sphere*>::iterator getIteratorSphere() { return spheres.begin(); }
    // std::vector<circle*>::iterator getIteratorCircle() { return circles.begin(); }
    // std::vector<line*>::iterator getIteratorLine() { return lines.begin(); }
    // std::vector<cube_bone*>::iterator getIteratorCube_bone() { return cube_bones.begin(); }

    std::string name;
    std::vector<sprite*> sprites;
    // std::vector<sphere*> spheres;
    // std::vector<circle*> circles;
    // std::vector<line*>   lines;
    // std::vector<cube_bone*> cube_bones;
};

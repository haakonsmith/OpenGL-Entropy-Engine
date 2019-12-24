#include "../Graphics/Renderable.hpp"
#include "../Physics/PhysicsObject.hpp"


#pragma once

class GameObject : Renderable, PhysicsObject {

public:
    GameObject();
    ~GameObject();
};
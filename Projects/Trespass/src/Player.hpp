#pragma once

#include <iostream>
#include "Game/GameObject.hpp"
class Player : public Entropy::GameObject{

public:

    void customPrePhysicsStep (double deltaTime) override {
        // cout << std::max(std::abs(velocity.x), std::abs(velocity.y)) << endl;
        // cout << velocity.x << endl;
        // cout << velocity.y << endl;
        // cout << velocity.z << endl;
        velocity = glm::normalize(velocity) * std::max(std::abs(velocity.x), std::abs(velocity.y));
    }

    Player();
    ~Player();
};

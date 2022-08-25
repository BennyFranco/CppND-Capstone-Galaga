#include "player_controller.h"
#include "ntime.h"
#include "renderer_locator.h"
#include <iostream>

using namespace nim;
using namespace galaga;

PlayerController::PlayerController() {
    name = "PlayerController";
    transform = nullptr;
    input = InputLocator::GetInput();
    velocity = 1;
}

PlayerController::PlayerController(const PlayerController &controller) {
    name = controller.name;
    transform = controller.transform;
    input = controller.input;
    velocity = controller.velocity;
}

PlayerController::PlayerController(PlayerController &&controller) {
    name = std::move(controller.name);
    transform = controller.transform;
    input = controller.input;
    velocity = controller.velocity;

    controller.transform = nullptr;
    controller.input = nullptr;
    controller.velocity = 0;
}

PlayerController::~PlayerController() {
    transform = nullptr;
    input = nullptr;
}

PlayerController &PlayerController::operator=(const PlayerController &controller) {
    if (&controller != this) {
        transform = nullptr;
        input = nullptr;

        name = controller.name;
        transform = controller.transform;
        input = controller.input;
        velocity = controller.velocity;
    }

    return *this;
}

PlayerController &PlayerController::operator=(PlayerController &&controller) {
    if (&controller != this) {
        transform = nullptr;
        input = nullptr;

        name = std::move(controller.name);
        transform = controller.transform;
        input = controller.input;
        velocity = controller.velocity;

        controller.transform = nullptr;
        controller.input = nullptr;
        controller.velocity = 0;
    }

    return *this;
}

void PlayerController::Update() {
    if (input->GetKeyDown(Key::LEFT) && !(transform->position.x < 0)) {
        transform->position.x -= velocity;
        transform->Position(&transform->position);
    }
    auto checkLeftLimit = !(transform->position.x > RendererLocator::GetRenderer()->Width() - transform->size.x);
    if (input->GetKeyDown(Key::RIGHT) && checkLeftLimit) {
        transform->position.x += velocity;
        transform->Position(&transform->position);
    }
}
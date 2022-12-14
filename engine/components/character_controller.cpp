#include "character_controller.h"

using namespace nim;

CharacterController::CharacterController() {
    name = "CharacterController";
    transform = nullptr;
    input = InputLocator::GetInput();
}

CharacterController::CharacterController(const CharacterController &controller) {
    name = controller.name;
    transform = controller.transform;
    input = controller.input;
}

CharacterController::CharacterController(CharacterController &&controller) noexcept {
    name = std::move(controller.name);
    transform = controller.transform;
    input = controller.input;

    controller.transform = nullptr;
    controller.input = nullptr;
}

CharacterController &CharacterController::operator=(const CharacterController &controller) {
    if (&controller != this) {
        transform = nullptr;
        input = nullptr;

        name = controller.name;
        transform = controller.transform;
        input = controller.input;
    }
    return *this;
}

CharacterController &CharacterController::operator=(CharacterController &&controller) noexcept {
    if (&controller != this) {

        transform = nullptr;
        input = nullptr;

        name = std::move(controller.name);
        transform = controller.transform;
        input = controller.input;

        controller.transform = nullptr;
        controller.input = nullptr;
    }
    return *this;
}

CharacterController::~CharacterController() {
    transform = nullptr;
    input = nullptr;
}

void CharacterController::Update() {
    if (input->GetKeyDown(Key::UP)) {
        transform->position.y -= 1.0f;
        transform->Position(&transform->position);
    }
    if (input->GetKeyDown(Key::DOWN)) {
        transform->position.y += 1.0f;
        transform->Position(&transform->position);
    }
    if (input->GetKeyDown(Key::LEFT)) {
        transform->position.x -= 1.0f;
        transform->Position(&transform->position);
    }
    if (input->GetKeyDown(Key::RIGHT)) {
        transform->position.x += 1.0f;
        transform->Position(&transform->position);
    }
}
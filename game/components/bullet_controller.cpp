#include "bullet_controller.h"
#include "animation_component.h"
#include "bullet.h"
#include "game_object.h"
#include "player_data.h"
#include <memory>
#include <random>

using namespace nim;
using namespace galaga;

BulletController::BulletController() {
    name = "BulletController";
    transform = nullptr;
    input = InputLocator::GetInput();
}

BulletController::BulletController(const BulletController &other) {
    name = other.name;
    transform = other.transform;
    input = other.input;
}

BulletController::BulletController(BulletController &&other) noexcept {
    name = std::move(other.name);
    transform = other.transform;
    input = other.input;

    other.transform = nullptr;
    other.input = nullptr;
}

BulletController::~BulletController() {
    transform = nullptr;
}

BulletController &BulletController::operator=(const BulletController &other) {
    if (&other != this) {
        transform = nullptr;
        input = nullptr;

        name = other.name;
        transform = other.transform;
        input = other.input;
    }

    return *this;
}

BulletController &BulletController::operator=(BulletController &&other) noexcept {
    if (&other != this) {
        transform = nullptr;
        input = nullptr;

        name = std::move(other.name);
        transform = other.transform;
        input = other.input;

        other.transform = nullptr;
        other.input = nullptr;
    }

    return *this;
}

void BulletController::Init() {
    shoot = parent->GetComponent<nim::SoundComponent<nim::Sound>>("SoundComponent");
}

void BulletController::Update() {
    if (input->GetKeyUp(Key::LEFT_CTRL) || input->GetKeyUp(Key::RIGHT_CTRL)) {
        Fire();
    }
}


void BulletController::Fire() {
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
    std::shared_ptr<AnimationComponent> graphicComponent = std::make_shared<AnimationComponent>("shot", true);
    graphicComponent->xOffset = 12;
    graphicComponent->yOffset = 0;
    graphicComponent->frames = 2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 1000000);

    Transform t(*transform);
    t.position.x += 24;
    t.position.y -= (t.size.y / 2.f) + 24;
    t.size.x = 12;
    t.size.y = 24;

    auto go = GameObject::Instantiate("bullet_" + std::to_string(distr(gen)), std::move(t), {graphicComponent, bullet});
    go->collisionLayer = Layer::Bullet;

    if (shoot != nullptr)
        shoot->Play();
}
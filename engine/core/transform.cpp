#include "transform.h"

using namespace nim;

Transform::Transform() : size(1.0f, 1.0f), position(0.f, 0.f) {}

Transform::Transform(Vector2 size, Vector2 position) {
    this->position = position;
    this->size = size;
}

Transform::Transform(const Transform &other) {
    size = other.size;
    position = other.position;
}

Transform::Transform(Transform &&other) {
    size = other.size;
    position = other.position;

    other.position = 0;
    other.size = 0;
}

void Transform::Size(Vector2 *size) {
    this->size.x = size->x;
    this->size.y = size->y;
}
void Transform::Size(Vector2 &&size) {
    this->size = size;
}
void Transform::Position(const Vector2 *position) {
    this->position.x = position->x;
    this->position.y = position->y;
}
void Transform::Position(Vector2 &&position) {
    this->position = position;
}
void Transform::Size(float width, float height) {
    size.x = width;
    size.y = height;
}
void Transform::Position(float x, float y) {
    position.x = x;
    position.y = y;
}
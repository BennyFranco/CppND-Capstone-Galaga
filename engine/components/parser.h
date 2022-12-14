#ifndef PARSER_H
#define PARSER_H

#include "animation_component.h"
#include "character_controller.h"
#include "collision_layer.h"
#include "game_object.h"
#include "sound_component.h"
#include "sprite_component.h"
#include "text_component.h"

namespace nim {
    class CustomComponentHelper {
    public:
        static CustomComponentHelper &Instance() {
            static std::unique_ptr<CustomComponentHelper> instance{new CustomComponentHelper()};
            return *instance;
        }
        Action<std::string, GameObject &, const YAML::Node &> DeserializeCustom;

    private:
        CustomComponentHelper() = default;
    };
}// namespace nim

namespace YAML {
    template<>
    struct convert<nim::GameObject> {
        static YAML::Node encode(const nim::GameObject &go) {
            YAML::Node node;
            node["type"] = "GameObject";
            node["name"] = go.name;
            node["transform"] = go.transform.get();
            for (auto &component: go.Components()) {
                if (component->name == "SpriteComponent") {
                    node["components"].push_back(dynamic_cast<nim::SpriteComponent *>(component.get()));
                } else if (component->name == "CharacterController") {
                    node["components"].push_back(dynamic_cast<nim::CharacterController *>(component.get()));
                } else if (component->name == "AnimationComponent") {
                    node["components"].push_back(dynamic_cast<nim::AnimationComponent *>(component.get()));
                } else {
                    // TODO:  Create some kind of serializer for custom components
                    // nim::DeserializeCustom.Invoke(go);
                }
            }
            return node;
        }

        static bool decode(const YAML::Node &node, nim::GameObject &go) {
            if (!node["type"]) return false;

            go.name = node["name"].as<std::string>();
            auto t = node["transform"].as<nim::Transform>();
            go.transform->size = t.size;
            go.transform->position = t.position;
            go.collisionLayer = (nim::Layer) node["layer"].as<uint>();

            if (node["components"].IsSequence()) {
                for (int i = 0; i < node["components"].size(); i++) {
                    auto goName = node["components"][i]["name"].as<std::string>();
                    if (goName == "SpriteComponent") {
                        go.AddComponent(std::make_shared<nim::SpriteComponent>(node["components"][i].as<nim::SpriteComponent>()));
                    } else if (goName == "CharacterController") {
                        auto sc = std::move(node["components"][i].as<nim::CharacterController>());
                        go.AddComponent(std::make_shared<nim::CharacterController>(sc));
                    } else if (goName == "AnimationComponent") {
                        go.AddComponent(std::make_shared<nim::AnimationComponent>(node["components"][i].as<nim::AnimationComponent>()));
                    } else if (goName == "TextComponent") {
                        go.AddComponent(std::make_shared<nim::TextComponent>(node["components"][i].as<nim::TextComponent>()));
                    } else if (goName == "SoundComponent") {
                        //                        auto soc = node["components"][i].as<nim::SoundComponent<nim::Sound>>();
                        nim::SoundComponent<nim::Sound> sSoundComponent;
                        if (convert<nim::SoundComponent<nim::Sound>>::decode(node["components"][i], sSoundComponent))
                            go.AddComponent(std::make_shared<nim::SoundComponent<nim::Sound>>(sSoundComponent));
                        else
                            go.AddComponent(std::make_shared<nim::SoundComponent<nim::Music>>(node["components"][i].as<nim::SoundComponent<nim::Music>>()));
                    } else {
                        nim::CustomComponentHelper::Instance().DeserializeCustom.Invoke(goName, go, node["components"][i]);
                    }
                }
            }

            return true;
        }
    };
}// namespace YAML
#endif
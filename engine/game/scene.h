#ifndef SCENE_H
#define SCENE_H

#include "asset_manager.h"
#include "game_object.h"
#include "parser.h"

#include <condition_variable>
#include <list>
#include <memory>
#include <string>

namespace nim {
    const std::string kScenesPath = "../scenes/";

    struct SceneData {
        std::string name;
        std::list<GameObject> gameObjects;
    };

    class Scene {
    public:
        Scene();
        Scene(const Scene &otherScene);
        Scene(Scene &&otherScene) noexcept;
        ~Scene();

        Scene &operator=(const Scene &otherScene);
        Scene &operator=(Scene &&otherScene) noexcept;

        void Init();
        void Update();
        void Quit();
        void Save(std::string filename = "");

        void RemoveGameObject(const std::string &name);
        void RemoveDirtyObjects();

        std::string Name() const { return sceneData->name; }
        SceneData *GetData() const { return sceneData.get(); }
        GameObject *AddGameObject(GameObject &&gameObject);
        SceneData *GetSceneData() { return sceneData.get(); }

        static std::unique_ptr<Scene> LoadScene(const std::string &sceneName);

    public:
        bool canDelete = true;

    private:
        std::unique_ptr<SceneData> sceneData;
        const uint fileVersion = 1;
        std::mutex mtx;
        std::condition_variable mtxCondition;
    };
}// namespace nim

namespace YAML {
    template<>
    struct convert<nim::SceneData> {
        static YAML::Node encode(const nim::SceneData &sd) {
            YAML::Node node;
            // TODO: implement encoder for nim::SceneData
            return node;
        }

        static bool decode(const YAML::Node &node, nim::SceneData &sd) {
            if (!node["name"]) return false;
            sd.name = node["name"].as<std::string>();

            std::list<nim::GameObject> gameObjects;
            for (auto i = node["gameObjects"].begin(); i != node["gameObjects"].end(); i++) {
                gameObjects.emplace_back(std::move(i->as<nim::GameObject>()));
            }

            sd.gameObjects = std::move(gameObjects);
            return true;
        }
    };
}// namespace YAML
#endif//SCENE_H
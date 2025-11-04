#include <raylib.h>

#include <vector>

#include "../levelData.h"
#include "../scene.h"

namespace synthrush {

class Entity;

class GameScene : public Scene {
   public:
    GameScene(Game *game, LevelData &data);
    ~GameScene();

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

    void SpawnEntity(Entity *ent);

    float mapMoveSpeed = 10;

   private:
    std::vector<Entity *> mEntities;

    LevelData mLevelData;
    int mLevelDataBeatIterator = 0;
    bool mDoneBeats = false;

    float mGameTime = 0;

    Ray mShootRay;
    Camera3D mCam;
};

}  // namespace synthrush

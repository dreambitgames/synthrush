#include <raylib.h>

#include <vector>

#include "../entities/enemy.h"
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

    void OnEnemyShot(int beatN);
    void OnEnemyMissed(int beatN);

    float mapMoveSpeed = 30;

   private:
    float mShootEffectFactor = 0;
    Color mScoreTextColor = WHITE;

    float mGameScore = 0;

    Sound mMusic;

    float CalculateShootScore(int beatN);
    void SpawnEnemyForBeatN(int beatN);

    std::vector<Enemy *> mEnemies;

    LevelData mLevelData;
    int mLevelDataBeatIterator = 0;

    bool mDoneBeats = false;
    bool mDoneMusic = false;

    float mGameTime = 0;

    Ray mShootRay;
    Camera3D mCam;

    friend class Enemy;
};

}  // namespace synthrush

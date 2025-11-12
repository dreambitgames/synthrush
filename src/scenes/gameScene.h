#include <raylib.h>

#include <vector>

#include "../entities/enemy.h"
#include "../levelData.h"
#include "../scene.h"
#include "../ui/button.h"

namespace synthrush {

class Entity;

class GameScene : public Scene {
   public:
    GameScene(Game *game, LevelData &data);
    ~GameScene();

    virtual void Initialize() override;

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

    void OnEnemyShot(int beatN);
    void OnEnemyMissed(int beatN);
    void OnGameOver();

    float mapMoveSpeed = 30;

   private:
    float GetAccuracyPercent();

    Vector2 mMousePos{};
    bool mPlayedWinSound = false;
    float mMapOff = 0;
    Vector2 mHudOffset{};
    float mGameOverCoefficient = 0;
    float mDisplayAccuracy = 0;
    int mLastIntDisplayAccuracy = 0;
    float mHelperCursorRadius = 0;
    float mLevelProgress = 0;

    int mAccuracyCount = 0;
    int mMissedCount = 0;

    float mFinalAccuracyPercent = 0;

    ui::Button mRetryBtn, mMenuBtn;

    bool mLostGame = false;
    bool mWonGame = false;
    float mScreenShowCountdown = 3;

    std::string mScoreIndicatorText;

    float mShootEffectFactor = 0;
    Color mScoreTextColor = WHITE;

    float mGameScore = 0;

    Sound mMusic, mGameOverSound, mBeepSound, mWinSound;

    float CalculateShootScore(int beatN);
    void SpawnEnemyForBeatN(int beatN);

    std::vector<Enemy *> mEnemies;

    LevelData mLevelData;
    int mLevelDataBeatIterator = 0;

    int mCurrentBeatIdx = 0;

    bool mDoneBeats = false;
    bool mDoneMusic = false;

    float mGameTime = 0;

    Ray mShootRay;
    Camera3D mCam{};

    float mCurrentShakeMagnitude = 0;
    float mCurrentShakeDuration = 0;
    float mCurrentShakeTimer = 0;

    friend class Enemy;
};

}  // namespace synthrush

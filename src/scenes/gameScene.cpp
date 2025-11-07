#include "gameScene.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <cstdio>
#include <string>

#include "../entities/enemy.h"
#include "../entity.h"
#include "../game.h"
#include "../random.h"

synthrush::GameScene::GameScene(Game *game, LevelData &data)
    : mLevelData(data),
      Scene(game),
      mRetryBtn(mGame, "Retry", 16, WHITE),
      mMenuBtn(mGame, "Menu", 16, WHITE) {
    mCam.position = {0, 5, 0};
    mCam.target = {0, 5, 10};
    mCam.up = {0, 1, 0};
    mCam.fovy = 45;
    mCam.projection = CAMERA_PERSPECTIVE;

    for (int i = 0; i < std::min((int)data.beats.size(), 3); ++i) {
        SpawnEnemyForBeatN(i);
        ++mLevelDataBeatIterator;
        if (mLevelDataBeatIterator >= mLevelData.beats.size())
            mDoneBeats = true;
    }

    mEnemies[0]->SetMarked();

    mMusic = LoadSound(data.audioFile.c_str());
    PlaySound(mMusic);

    mGameOverSound = LoadSound("assets/sfx/gameOver.wav");
}

synthrush::GameScene::~GameScene() { UnloadSound(mMusic); }

void synthrush::GameScene::SpawnEnemyForBeatN(int beatN) {
    float posX, posY;

    if (util::Random(0, 1) > 0.5f)
        posX = util::Random(-5, -1);
    else
        posX = util::Random(1, 5);

    if (util::Random(0, 1) > 0.5f)
        posY = util::Random(1, 5);
    else
        posY = util::Random(6, 7);

    mEnemies.push_back(new Enemy(
        this, {posX, posY, 15 + mapMoveSpeed * mLevelData.beats[beatN] - mapMoveSpeed * mGameTime},
        beatN));
}

void synthrush::GameScene::Update(float dT) {
    static Vector2 mousePos{};
    Vector2 mousePosNormalied = {2 * mousePos.x / mGame->screenW - 1,
                                 2 * mousePos.y / mGame->screenH - 1};
    Vector3 camTarget = {-mousePosNormalied.x, 5 - mousePosNormalied.y, 10};

    mCam.target = Vector3Lerp(mCam.target, camTarget, 7 * dT);
    // UpdateCamera(&mCam, CAMERA_FREE);

    mShootRay = GetMouseRay(mousePos, mCam);

    for (Entity *ent : mEnemies) ent->Update(dT);

    if (!mGameOver) {
        mousePos = GetMousePosition();

        mGameTime += dT;

        if (!mDoneBeats)
            if (mGameTime >= mLevelData.beats[mLevelDataBeatIterator] - 3 - 30 / mapMoveSpeed) {
                SpawnEnemyForBeatN(mLevelDataBeatIterator);
                ++mLevelDataBeatIterator;
                if (mLevelDataBeatIterator >= mLevelData.beats.size())
                    mDoneBeats = true;
            }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            mEnemies[0]->CheckHit(mShootRay);
    } else {
    }

    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
                                  [](Enemy *e) {
                                      if (e->NeedsDeletion()) {
                                          delete e;
                                          return true;
                                      }
                                      return false;
                                  }),
                   mEnemies.end());

    mShootEffectFactor = Lerp(mShootEffectFactor, 0, 3 * dT);
    mScoreTextColor = ColorLerp(mScoreTextColor, WHITE, 3 * dT);

    if (mGameOver)
        mapMoveSpeed = Lerp(mapMoveSpeed, 0, 0.7 * dT);
}

float synthrush::GameScene::CalculateShootScore(int beatN) {
    return (0.5 - Clamp(abs(mGameTime - mLevelData.beats[beatN]), 0, 1)) * 2;
}

void synthrush::GameScene::OnEnemyShot(int beatN) {
    if (mGameOver)
        return;

    if (mEnemies.size() > 1)
        mEnemies[1]->SetMarked();

    float shootScore = CalculateShootScore(beatN);
    mGameScore += shootScore;

    mShootEffectFactor += 1;

    if (shootScore >= 0.9) {
        mScoreTextColor = MAGENTA;
        mScoreIndicatorText = "PERFECT!!!";
    } else if (shootScore >= 0.7) {
        mScoreTextColor = GREEN;
        mScoreIndicatorText = "NICE!";
    } else if (shootScore >= 0.5) {
        mScoreTextColor = YELLOW;
        mScoreIndicatorText = "OKAY";
    } else {
        mScoreTextColor = RED;
        mScoreIndicatorText = "OOPS...";
    }

    if (mGameScore <= 0 && !mGameOver)
        OnGameOver();
}

void synthrush::GameScene::OnGameOver() {
    StopSound(mMusic);
    mGameOver = true;
    PlaySound(mGameOverSound);
}

void synthrush::GameScene::OnEnemyMissed(int beatN) {
    if (mGameOver)
        return;
    if (mEnemies.size() > 1)
        mEnemies[1]->SetMarked();
    mGameScore -= 1;

    mShootEffectFactor = 0.7f;
    mScoreTextColor = RED;

    mScoreIndicatorText = "OOPS...";

    if (mGameScore <= 0 && !mGameOver)
        OnGameOver();
}

static void DrawGroundGrid(float off) {
    const float gridSpacing = 3.0f;
    const int gridCount = 50;
    const int gridCountSides = 10;
    const float halfWidth = (gridCountSides / 2.0f) * gridSpacing;

    Color color = Fade(MAGENTA, Clamp(off / 10, 0, 1));

    off = fmodf(off, gridSpacing);

    for (int i = -gridCountSides / 2; i <= gridCountSides / 2; ++i) {
        float x = i * gridSpacing;
        DrawLine3D({x, 0, -gridSpacing * gridCount}, {x, 0, gridSpacing * gridCount}, color);
    }

    for (int i = -gridCount; i <= gridCount; ++i) {
        float z = i * gridSpacing - off;
        DrawLine3D({-halfWidth, 0, z}, {halfWidth, 0, z}, color);
    }
}

void synthrush::GameScene::Render(float dT) {
    ClearBackground(BLACK);
    BeginMode3D(mCam);

    static float off = 0;
    DrawGroundGrid(off);
    off += dT * mapMoveSpeed;

    for (Entity *ent : mEnemies) ent->Render(dT);

    EndMode3D();

    static Vector2 hudOffset{};
    if (mGameOver)
        hudOffset = Vector2Lerp(hudOffset, {0, 0}, 0.7 * dT);
    else
        hudOffset = Vector2Lerp(hudOffset, Vector2Scale(GetMouseDelta(), -0.1f), 5 * dT);

    DrawTextEx(mGame->mainFont, std::to_string(mGameScore).c_str(), Vector2Add(hudOffset, {10, 10}),
               mShootEffectFactor * 10 + 24, 0, mScoreTextColor);

    DrawTextPro(mGame->mainFont, mScoreIndicatorText.c_str(), Vector2Add(hudOffset, {70, 42}),
                {0, 0}, -5, mShootEffectFactor * 6 + 12, 0,
                Fade(mScoreTextColor, mShootEffectFactor));

    static float gameOverCoefficient = 0;
    if (mGameOver) {
        gameOverCoefficient = Lerp(gameOverCoefficient, 1, 3 * dT);
        DrawRectangle(0, 0, mGame->screenW, mGame->screenH, Fade(BLACK, gameOverCoefficient * 0.7));

        const Vector2 textDim = MeasureTextEx(mGame->mainFont, "Game Over!", 32, 0);

        DrawTextEx(mGame->mainFont, "Game Over!",
                   {mGame->virtualW / 2.0f - textDim.x / 2,
                    mGame->virtualH / 2.0f - textDim.y / 2 - 30 + gameOverCoefficient * 30},
                   32, 0, Fade(RED, gameOverCoefficient * 3));

        mMenuBtn.Render({(float)mGame->virtualW / 2, (float)mGame->virtualH - 25});
        mRetryBtn.Render({(float)mGame->virtualW / 2, (float)mGame->virtualH - 50});
    }
}

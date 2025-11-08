#include "gameScene.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <cstdio>
#include <string>

#include "../audioProc.h"
#include "../entities/enemy.h"
#include "../entity.h"
#include "../game.h"
#include "../random.h"

synthrush::GameScene::GameScene(Game *game, LevelData &data)
    : mLevelData(data),
      Scene(game),
      mRetryBtn(mGame, "Retry", 16, WHITE),
      mMenuBtn(mGame, "Menu", 16, WHITE) {
    mMenuBtn.centerPosition = {(float)mGame->virtualW / 2, (float)mGame->virtualH - 25};
    mRetryBtn.centerPosition = {(float)mGame->virtualW / 2, (float)mGame->virtualH - 50};

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

    HideCursor();
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
    if (!mLostGame && !mWonGame)
        mousePos = GetMousePosition();

    Vector2 mousePosNormalied = {2 * mousePos.x / mGame->screenW - 1,
                                 2 * mousePos.y / mGame->screenH - 1};
    Vector3 camTarget = {-mousePosNormalied.x, 5 - mousePosNormalied.y, 10};

    mCam.target =
        Vector3Lerp(mCam.target, mWonGame ? Vector3{0, 5, 10} : camTarget, (mWonGame ? 1 : 7) * dT);

    if (mCurrentShakeTimer > 0) {
        float magnitude = mCurrentShakeMagnitude * (mCurrentShakeTimer / mCurrentShakeDuration);
        Vector3 offset = {util::Random(-magnitude, magnitude), util::Random(-magnitude, magnitude),
                          util::Random(-magnitude, magnitude)};
        mCam.position = Vector3Add({0, 5, 0}, offset);
        mCam.target = Vector3Add(mCam.target, offset);
        mCurrentShakeTimer -= dT;
    } else {
        mCam.position = {0, 5, 0};
    }

    mShootRay = GetMouseRay(mousePos, mCam);

    for (Entity *ent : mEnemies) ent->Update(dT);

    if (!mLostGame && !mWonGame) {
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

    if (mLostGame || mWonGame) {
        mapMoveSpeed = Lerp(mapMoveSpeed, 0, 0.7 * dT);
        if (mScreenShowCountdown > 0)
            mScreenShowCountdown -= dT;

        // temporary
        if (mMenuBtn.Clicked())
            mGame->EndGame();
    }
}

float synthrush::GameScene::CalculateShootScore(int beatN) {
    return (0.5 - Clamp(abs(mGameTime - mLevelData.beats[beatN]), 0, 1)) * 2;
}

void synthrush::GameScene::OnEnemyShot(int beatN) {
    if (mLostGame || mWonGame)
        return;

    if (mEnemies.size() > 1)
        mEnemies[1]->SetMarked();

    float shootScore = CalculateShootScore(beatN);
    mGameScore += shootScore;

    mShootEffectFactor += 1;

    ++mCurrentBeatIdx;

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

    if (mCurrentBeatIdx >= mLevelData.beats.size() && mGameScore > 0)
        mWonGame = true;

    if (mGameScore <= 0 && !mLostGame)
        OnGameOver();
}

void synthrush::GameScene::OnGameOver() {
    StopSound(mMusic);
    mLostGame = true;
    PlaySound(mGameOverSound);

    mCurrentShakeTimer = mCurrentShakeDuration = 1;
    mCurrentShakeMagnitude = 1;
}

void synthrush::GameScene::OnEnemyMissed(int beatN) {
    if (mLostGame || mWonGame)
        return;
    if (mEnemies.size() > 1)
        mEnemies[1]->SetMarked();
    mGameScore -= 1;

    mShootEffectFactor = 0.7f;
    mScoreTextColor = RED;

    mScoreIndicatorText = "OOPS...";

    ++mCurrentBeatIdx;

    if (mCurrentBeatIdx >= mLevelData.beats.size() && mGameScore > 0)
        mWonGame = true;

    if (mGameScore <= 0 && !mLostGame && !mWonGame)
        OnGameOver();

    mCurrentShakeTimer = mCurrentShakeDuration = 0.5;
    mCurrentShakeMagnitude = 0.1;
}

static void DrawSides(float *samplesL, float *samplesR, int sampleCount) {
    const float lineSpacing = 3.0f;
    const int lineCount = 3;
    const int gridCount = 50;
    const int gridCountSides = 10;
    const float gridSpacing = 3.0f;
    const float halfWidth = (lineCount / 2.0f) * lineSpacing;
    const int linePartCount = sampleCount;

    const float waveAmplCoeff = 30;

    Color color = BLUE;

    for (int sideCoeff = -1; sideCoeff < 2; sideCoeff += 2) {
        float *samples = sideCoeff == -1 ? samplesL : samplesR;

        for (int i = 0; i <= lineCount; ++i) {
            float x = sideCoeff * (i * lineSpacing + gridCountSides * (gridSpacing + 1) / 2);

            float step = gridCount * gridSpacing / linePartCount;
            for (int c = 0; c < linePartCount; ++c)
                DrawLine3D({x, samples[c] * waveAmplCoeff / (i * 3 + 1), step * c},
                           {x, samples[c + 1] * waveAmplCoeff / (i * 3 + 1), step * (c + 1)},
                           Fade(color, samples[c] * 10 + 0.5f));
        }
    }
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

    DrawSides(GetAmplitudesL(), GetAmplitudesR(), 25);

    for (Entity *ent : mEnemies) ent->Render(dT);

    EndMode3D();

    Vector2 virtualMousePos =
        Vector2Scale(GetMousePosition(), mGame->virtualW / (float)mGame->screenW);

    static Vector2 hudOffset{};
    if (mLostGame || mWonGame)
        hudOffset = Vector2Lerp(hudOffset, {0, 0}, 0.7 * dT);
    else
        hudOffset = Vector2Lerp(hudOffset, Vector2Scale(GetMouseDelta(), -0.1f), 5 * dT);

    DrawTextEx(mGame->mainFont, std::to_string(mGameScore).c_str(), Vector2Add(hudOffset, {10, 10}),
               mShootEffectFactor * 10 + 24, 0, mScoreTextColor);

    DrawTextPro(mGame->mainFont, mScoreIndicatorText.c_str(), Vector2Add(hudOffset, {70, 42}),
                {0, 0}, -5, mShootEffectFactor * 6 + 12, 0,
                Fade(mScoreTextColor, mShootEffectFactor));

    static float gameOverCoefficient = 0;

    if (mLostGame && mScreenShowCountdown <= 0) {
        gameOverCoefficient = Lerp(gameOverCoefficient, 1, 3 * dT);
        DrawRectangle(0, 0, mGame->screenW, mGame->screenH, Fade(BLACK, gameOverCoefficient * 0.7));

        const Vector2 textDim = MeasureTextEx(mGame->mainFont, "Game Over!", 32, 0);

        DrawTextEx(mGame->mainFont, "Game Over!",
                   {mGame->virtualW / 2.0f - textDim.x / 2,
                    mGame->virtualH / 2.0f - textDim.y / 2 - 30 + gameOverCoefficient * 30},
                   32, 0, Fade(RED, gameOverCoefficient * 3));

        mMenuBtn.Render();
        mRetryBtn.Render();
    } else if (mWonGame && mScreenShowCountdown <= 0) {
        gameOverCoefficient = Lerp(gameOverCoefficient, 1, 3 * dT);
        DrawRectangle(0, 0, mGame->screenW, mGame->screenH, Fade(BLACK, gameOverCoefficient * 0.7));

        const Vector2 textDim = MeasureTextEx(mGame->mainFont, "Level Complete!", 32, 0);

        DrawTextEx(mGame->mainFont, "Level Complete!",
                   {mGame->virtualW / 2.0f - textDim.x / 2,
                    mGame->virtualH / 2.0f - textDim.y / 2 - 30 + gameOverCoefficient * 30},
                   32, 0, Fade(GREEN, gameOverCoefficient * 3));

        mMenuBtn.Render();
        mRetryBtn.Render();
    }

    float helperCursorRadiusTarget = 5;

    float beatCloseIndicator =
        (mCurrentBeatIdx < mLevelData.beats.size() && !mLostGame && !mWonGame)
            ? Clamp(mLevelData.beats[mCurrentBeatIdx] - mGameTime, 0, 10)
            : 0;

    helperCursorRadiusTarget += 10 * beatCloseIndicator;

    static float helperCursorRadius = 0;
    helperCursorRadius = Lerp(helperCursorRadius, helperCursorRadiusTarget, 10 * dT);

    DrawCircle(virtualMousePos.x, virtualMousePos.y, helperCursorRadius,
               Fade(WHITE, Clamp(1 - beatCloseIndicator, 0, 0.7f) + 0.3f));
}

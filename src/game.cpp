#include "game.h"

#include <raylib.h>
#include <raymath.h>

#include <cmath>
#include <fstream>

#include "audioProc.h"
#include "levelData.h"
#include "scenes/gameScene.h"

synthrush::Game::Game(int w, int h, int vw, int vh)
    : screenW(w), screenH(h), virtualW(vw), virtualH(vh) {
    InitWindow(w, h, "[SYNTHRUSH]");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    synthrush::InitAudioProcessor();

    mainFont = LoadFont("assets/fonts/zendots.ttf");

    std::ifstream f("data/levels/test.json");
    json data = json::parse(f);
    LevelData testLevel(data);

    ChangeScene(new GameScene(this, testLevel));
}

synthrush::Game::~Game() {
    if (mCurrentScene)
        delete mCurrentScene;
    UnloadFont(mainFont);
    CloseWindow();

    synthrush::DeinitAudioProcessor();
    CloseAudioDevice();
}

void synthrush::Game::Update(float dT) {
    mCurrentScene->Update(dT);

    if (mSceneTransitioning) {
        if (mSceneTransitioningTime >= mSceneTransitioningMaxTime / 2) {
            ChangeScene(mSceneTransitioningTo);
            mSceneTransitioning = false;
        }

        mSceneTransitioningTime += dT;
    }
}

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    mCurrentScene->Render(dT);

    if (mSceneTransitioning) {
        float coeff;

        if (mSceneTransitioningTime <= mSceneTransitioningMaxTime / 2)
            coeff = 2 * mSceneTransitioningTime / mSceneTransitioningMaxTime;
        else
            coeff = -2 * mSceneTransitioningTime / mSceneTransitioningMaxTime + 2;

        DrawRectangle(0, 0, virtualW, virtualH, Fade(BLACK, coeff));
    }
    EndDrawing();
}

void synthrush::Game::ChangeSceneTransition(Scene *newScene) {
    if (mSceneTransitioning) {
        delete newScene;
        return;
    }

    mSceneTransitioning = true;
    mSceneTransitioningTime = 0;
    mSceneTransitioningTo = newScene;
}

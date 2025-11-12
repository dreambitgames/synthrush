#include "button.h"

#include <raylib.h>
#include <raymath.h>

#include "../game.h"

synthrush::ui::Button::Button(Game *game, const char *text, int fontSize, Color color)
    : text(text), fontSize(fontSize), color(color), mGame(game) {
    mTextDimensions = MeasureTextEx(game->mainFont, this->text.c_str(), fontSize, 0);
}

void synthrush::ui::Button::Render() {
    Rectangle rect =
        Rectangle{centerPosition.x - mTextDimensions.x / 2,
                  centerPosition.y - mTextDimensions.y / 2, mTextDimensions.x, mTextDimensions.y};

    bool hover = CheckCollisionPointRec(
        Vector2Scale(GetMousePosition(), mGame->virtualW / (float)mGame->screenW), rect);
    float alpha = hover ? (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? 0.3 : 1) : 0.7;

    DrawTextEx(mGame->mainFont, text.c_str(),
               {centerPosition.x - mTextDimensions.x / 2, centerPosition.y - mTextDimensions.y / 2},
               fontSize, 0, Fade(color, alpha));
}

bool synthrush::ui::Button::Clicked() {
    Rectangle rect =
        Rectangle{centerPosition.x - mTextDimensions.x / 2,
                  centerPosition.y - mTextDimensions.y / 2, mTextDimensions.x, mTextDimensions.y};

    bool hover = CheckCollisionPointRec(
        Vector2Scale(GetMousePosition(), mGame->virtualW / (float)mGame->screenW), rect);
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT) && hover;
}

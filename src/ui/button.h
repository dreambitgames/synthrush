#pragma once

#include <raylib.h>

#include <string>

namespace synthrush {

class Game;

namespace ui {
class Button {
   public:
    Button(Game *game, const char *text, int fontSize, Color color);

    void Render();
    bool Clicked();

    std::string text;
    int fontSize;
    Color color;
    Vector2 centerPosition;

   private:
    Game *mGame = nullptr;
    Vector2 mTextDimensions;
};

}  // namespace ui
}  // namespace synthrush

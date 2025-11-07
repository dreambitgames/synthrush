#pragma once

#include <raylib.h>

#include <string>

namespace synthrush {

class Game;

namespace ui {
class Button {
   public:
    Button(Game *game, const char *text, int fontSize, Color color);

    void Render(Vector2 centerPosition);
    bool Clicked(Vector2 centerPosition);

    std::string text;
    int fontSize;
    Color color;

   private:
    Game *mGame = nullptr;
    Vector2 mTextDimensions;
};

}  // namespace ui
}  // namespace synthrush

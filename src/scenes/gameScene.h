#include <raylib.h>

#include "../scene.h"

namespace synthrush {

class GameScene : public Scene {
   public:
    GameScene(Game *game);
    ~GameScene();

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

    float mapMoveSpeed = 10;

   private:
    Ray mShootRay;
    Camera3D mCam;
};

}  // namespace synthrush

#ifndef ST_GAME_H_INCLUDED
#define ST_GAME_H_INCLUDED

#include "Nutmeg.h"
#include "CState.h"
#include "game.h"

class ST_Game : public CState
{
    public:

        ST_Game( Nutmeg::Engine *eng ) : CState(eng) { isActivated = false; }

        void onMouseDown(int x, int y, int b);
		void onRender();
		void onShutdown();
		void onKeyDown(int key);
		void onStartup(int argc, const char **argv);
		void onUpdate(float dt);

        void onActivateState();
        void onDeactivateState();

    private:

        Game* game;

        bool isActivated;

        Nutmeg::CameraNode* camera;
        Nutmeg::Scene *scene;

};

#endif // ST_GAME_H_INCLUDED

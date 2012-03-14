#ifndef ST_AUTHOR_H_INCLUDED
#define ST_AUTHOR_H_INCLUDED

#include "Nutmeg.h"
#include "CState.h"



class ST_Author : public CState
{
    public:

        ST_Author( Nutmeg::Engine *eng ) : CState(eng) { isActivated = false; }

        void onMouseDown(int x, int y, int b);
		void onRender();
		void onShutdown();
		void onStartup(int argc, const char **argv);
		void onUpdate(float dt);

        void onActivateState();
        void onDeactivateState();

    private:

        bool isActivated;

        Nutmeg::CameraNode* camera;
        Nutmeg::Scene *scene;

};

#endif // ST_AUTHOR_H_INCLUDED

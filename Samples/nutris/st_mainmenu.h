#ifndef ST_MAINMENU_H_INCLUDED
#define ST_MAINMENU_H_INCLUDED

#include "Nutmeg.h"
#include "CState.h"

// id gui элементов для обработчика событий
enum
{
        MM_IDBUTTON_GAME = 1,
        MM_IDBUTTON_LEVELMANAGE,
        MM_IDBUTTON_OPTIONS,
        MM_IDBUTTON_EXIT,
};

class ST_MainMenu : public CState
{
    public:

        ST_MainMenu( Nutmeg::Engine *eng ) : CState(eng) { isActivated = false; }

        void onMouseDown(int x, int y, int b);
        void onMouseMove(int x, int y);
		void onRender();
		void onShutdown();
		void onStartup(int argc, const char **argv);
		void onUpdate(float dt);

        void onActivateState();
        void onDeactivateState();

    private:

        bool isActivated;

        // текущие координаты мыши
        int mx;
        int my;

        Nutmeg::CameraNode* camera;
        Nutmeg::Scene *scene;

        float rotateStart;
        float rotateAbout;
        float rotateExit;

};

#endif // ST_MAINMENU_H_INCLUDED

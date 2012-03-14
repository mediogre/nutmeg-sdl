#ifndef CSTATE_H
#define CSTATE_H

#include "Nutmeg.h"


class CState : public Nutmeg::Application, Nutmeg::SceneEventListener
{
    public:
        CState(Nutmeg::Engine * eng) { engine = eng; }

        virtual void onActivate(bool flag) {}
		virtual void onError(const char * err) {}
		virtual void onKeyChar(char c) {}
        virtual void onKeyDown(int key) {}
		virtual void onKeyRepeat(int key) {}
		virtual void onKeyUp(int key) {}
        virtual void onMouseDown(int x, int y, int b) {}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseUp(int x, int y, int b) {}
        virtual void onRender() {}
        virtual void onResize(int w, int h) {}
        virtual void onShutdown() {}
        virtual void onStartup(int argc, const char **argv) {}
        virtual void onTriggerEvenet(Nutmeg::TriggerNode *t0, Nutmeg::TriggerNode *t1, int event) {}
        virtual void onUpdate(float dt) {}

        virtual void onActivateState() {}
        virtual void onDeactivateState() {}

    protected:

        Nutmeg::Engine * engine;
};

#endif // CSTATE_H

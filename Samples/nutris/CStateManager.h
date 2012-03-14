#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

#include "Nutmeg.h"

#include <map>
#include "st_conf.h"
#include "CState.h"

class CStateManager  : public Nutmeg::Application, Nutmeg::SceneEventListener
{
    typedef std::map< game::STATES , CState* > States_t;
    typedef std::map< game::STATES , CState* >::iterator iter_t;

    public:
        CStateManager(Nutmeg::Engine * eng)
        {
            MainState = NULL;
            NextState = game::STATE_NONE;
            isActivateNeeded = false;
            initSystemVariables(eng);
        }

        virtual void onActivate(bool flag) { MainState->onActivate(flag); }
		virtual void onError(const char * err) { }
		virtual void onKeyChar(char c) { MainState->onKeyChar(c); }
        virtual void onKeyDown(int key) { MainState->onKeyDown(key); }
		virtual void onKeyRepeat(int key) { MainState->onKeyRepeat(key); }
		virtual void onKeyUp(int key) { MainState->onKeyUp(key); }
        virtual void onMouseDown(int x, int y, int b) { MainState->onMouseDown(x,y,b); }
        virtual void onMouseMove(int x, int y) { MainState->onMouseMove(x,y); }
        virtual void onMouseUp(int x, int y, int b) { MainState->onMouseUp(x,y,b); }
        virtual void onRender() { MainState->onRender(); }
        virtual void onResize(int w, int h) { if (MainState) MainState->onResize(w,h); }

        virtual void onShutdown()
        {
            // удаляем зарегистрированные состояния
            for ( iter_t iter = States.begin(); iter != States.end(); iter++ )
            {
                (*iter).second->onShutdown();
                delete (*iter).second;
            }
            States.clear();
        }

        virtual void onStartup(int argc, const char **argv)
        {
			platform->setVideoMode(Nutmeg::VideoMode(640, 480, 32), false);
            for ( iter_t iter = States.begin(); iter != States.end(); iter++ )
            {
                (*iter).second->onStartup(argc, argv);
            }
        }

        virtual void onTriggerEvenet(Nutmeg::TriggerNode *t0, Nutmeg::TriggerNode *t1, int event) { MainState->onTriggerEvenet(t0,t1,event); }

        virtual void onUpdate(float dt)
        {
            if (isActivateNeeded)
            {
                switchState(NextState);
                isActivateNeeded = false;
                return;
            }

            platform->setTitle(Nutmeg::format("fps: %.2f", platform->getFPS()));
            MainState->onUpdate(dt);
        }

        // регистрация состояния в массиве состояний States
        void registerState(game::STATES n, CState* s)
        {
            // если такое состояние уже есть, то грохаем его предварительно
            // в принципе можно и не проверять, просто внимательнее регистрировать
            if (States.find(n) != States.end())
            {
                delete States[n];
                States.erase(n);
            }
            States[n] = s; // добавляем состояние
        }//registerState

        // устанавливает следующее за текущим состояние
        // на следующем onUpdate будет выполнено переключение
        void setNextState(game::STATES n)
        {
            NextState = n;
            isActivateNeeded = true;
        }

    protected:
        States_t States; // пул состояний

        CState* MainState; // главное состояние
        game::STATES NextState; // следующее за главным состояние

    private:

        bool isActivateNeeded;

        // переключаем состояние
		// !!! а как бы вот так вот подумать над тем, чтобы избавиться от game::STATES и использовать string для идентификации и обращеню к стейтам?
		// головную боль снимает как рукой.
        void switchState(game::STATES n)
        {
//            #ifdef _DEBUG
//            console->echo( format("StateManager::switchState - %s\n", state2str(n)) );
//            #endif

            if (n == game::STATE_NONE) // запрошен сброc текущего состояния, т.е. у меня равносильно EXIT
            {
                engine->halt();
            }
            else //иначе записываем в текущее состояние запрошенное из пула и активируем его
            {
                if (MainState) MainState->onDeactivateState(); // если текущее состояние установлено - деактивируем его
                //TODO: выполнить проверку на существование состояния, а то вот прямо ща заказал переключение
                // на состояние которое не только не реализовано, а да же в States осутствует :)
                MainState = States[n];
                MainState->onActivateState();
            }
        }//switchState
};

extern CStateManager* stmgr;

#endif // CSTATEMANAGER_H

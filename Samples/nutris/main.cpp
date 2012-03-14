//------------------------------------------------------------------------------
//
// Nutmeg::Core::Engine
//
// copyright:	(c) 2008-2011 One More Studio
// author:		Alexey Egorov (FadeToBlack aka EvilSpirit)
// mail:		anandamide@mail.ru
// icq:			455-761-951
// skype:		fade_to_evil
//
//------------------------------------------------------------------------------

#include "Nutmeg.h"
#include "RenderOpenGL1.h"
#include "PlatformSDL.h"
#include "AudioSquall.h"

#include "CStateManager.h"
#include "st_mainmenu.h"
#include "st_game.h"
#include "st_author.h"

CStateManager* stmgr;

//------------------------------------------------------------------------------

namespace Nutmeg {

	int engineMain(int argc, const char **argv) {

		if (isPlatformSDLSupported() == true) {
			AbstractPlatform::addImplementation("SDL", createPlatformSDL);
		}

        if (isAudioSquallSupported() == true) {
            AbstractAudio::addImplementation("Squall", createAudioSquall);
        }

		Engine engine;

		engine.setSubsystemCoreId("SDL");
		engine.setSubsystemRenderId("OpenGL1");
        engine.setSubsystemAudioId("Squall");

		stmgr = new CStateManager(&engine);
        stmgr->registerState( game::STATE_MAINMENU, new ST_MainMenu(&engine) );
        stmgr->registerState( game::STATE_ABOUT, new ST_Author(&engine) );
        stmgr->registerState( game::STATE_GAME, new ST_Game(&engine) );
        stmgr->setNextState( game::STATE_MAINMENU );

        Log::open("nutris.log");

		return engine.execute(stmgr, argc, argv);
	}

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------


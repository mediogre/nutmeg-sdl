//------------------------------------------------------------------------------
//
// Nutmeg::Core::Engine
//
// copyright:	(c) 2008-2011 One More Studio
// author:		Alexey Egorov (FadeToBlack aka EvilSpirit)
// url:			http://nutmeg-engine.ru
// mail:		anandamide@mail.ru
// icq:			455-761-951
// skype:		fade_to_evil
//------------------------------------------------------------------------------

#ifndef PlatformSDLH
#define PlatformSDLH

namespace Nutmeg {

	class AbstractPlatform;
	class Engine;
	
	bool isPlatformSDLSupported();
	AbstractPlatform *createPlatformSDL(Engine *engine);
}

#endif

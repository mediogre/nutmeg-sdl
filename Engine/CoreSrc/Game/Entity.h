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
//
//------------------------------------------------------------------------------

#ifndef EntityH
#define EntityH

//------------------------------------------------------------------------------

#include "Game.h"
#include "Entity.h"
#include "LibApi.h"

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------

	class Game;
	class Level;
	class EntityNode;
	class AbstractPlatform;

	//--------------------------------------------------------------------------
	//
	// class Entity
	//
	//--------------------------------------------------------------------------

	class NUTMEG_API Entity {

	protected:

		Game *game;
		Level *level;
		EntityNode *node;
		bool is_destroyed;

		virtual void onCreate() { }
		virtual void onAfterCreate() { }
		virtual bool onUpdate(float dt) { return true; }
		virtual void onDestroy() { }

		virtual void onActivate() { }
		virtual void onDeactivate() { }

		virtual void onMouseDown(int x, int y, int b) { }
		virtual void onMouseUp(int x, int y, int b) { }
		virtual void onMouseMove(int x, int y) { }
		virtual void onKeyDown(int key) { }
		virtual void onKeyUp(int key) { }
		virtual void onKeyRepeat(int key) { }
		virtual void onKeyChar(char c) { }
		virtual void onControl(AbstractPlatform *platform) { }

		virtual void onRenderDebug(AbstractRender *render) { }
		virtual void onRender2d(AbstractRender *render) { }

	public:

		Entity();
		virtual ~Entity();

		const char *getName() const;
		const char *getTypeName() const;

		void create(Level *level_, EntityNode *node_);
		void afterCreate();
		bool update(float dt);
		void destroy();

		void activate();
		void deactivate();

		void mouseDown(int x, int y, int b);
		void mouseUp(int x, int y, int b);
		void mouseMove(int x, int y);
		void keyDown(int key);
		void keyUp(int key);
		void keyRepeat(int key);
		void keyChar(char c);
		void control(AbstractPlatform *platform);

		void renderDebug(AbstractRender *render);
		void render2d(AbstractRender *render);

		EntityNode *getNode();
		Level *getLevel();
		Game *getGame();

		bool isDestroyed() const;

	};

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------

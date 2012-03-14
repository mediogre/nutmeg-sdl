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

#include "PlatformSDL.h"

//------------------------------------------------------------------------------

#include <assert.h>
#include "Str.h"
#include "Engine.h"
#include "Log.h"
#include "Console.h"
#include "AbstractRender.h"

//------------------------------------------------------------------------------

#ifdef NUTMEG_PLATFORM_SDL

//------------------------------------------------------------------------------

#include "SDL.h"
#include "SDL_opengl.h"
#include "Timer.h"
#include "AbstractPlatform.h"

namespace Nutmeg {
	class PlatformSDL : public AbstractPlatform {
        float    updateFPS;
        FPSTimer fps;
        FPSTimer updateTimer;
		FPSTimer renderTimer;
		Application *application;

		bool started;
        bool initialized;

        SDL_Surface *screen_;

        VideoMode vm_;
        bool full_screen_;

        TextureRef cursor;
	public:
		PlatformSDL(Engine *engine) : AbstractPlatform(engine) {
			initialized = false;
		}

		virtual ~PlatformSDL() { }

		virtual int main(Application *application, int argc, const char **argv);

		virtual void onStartup(int argc, const char **argv);

		//----------------------------------------------------------------------
		// perfomance control/info
		//----------------------------------------------------------------------

		virtual float getFPS() const; 
		virtual void clearTimer() const;

		virtual float getRenderTime() const;
		virtual float getUpdateTime() const;
		virtual float getFrameTime() const;

		virtual float getUpdateFPS(); 
		virtual void setUpdateFPS(float fps); 
		virtual float getDt() const; 
		virtual float getTimer(); 

		//----------------------------------------------------------------------
		// video
		//----------------------------------------------------------------------

		virtual int getVideoModesCount() const; 
		virtual VideoMode getVideoMode(int) const; 

		virtual VideoMode getCurrentVideoMode() const; 
		virtual bool isFullscreen() const; 

		virtual bool setVideoMode(const VideoMode &, bool); 

		virtual int getWidth() const; 
		virtual int getHeight() const; 
		virtual float getAspectRatio() const; 

		virtual void setOritationMode(VideoOrientationType omt); 
		virtual VideoOrientationType getOritationMode() const; 

		virtual void onRender();

		//----------------------------------------------------------------------
		// input
		//----------------------------------------------------------------------

		virtual const char *getKeyName(int) const;
		virtual int getKeyByName(const char *name) const;
		virtual bool keyHold(int key) const;

		virtual bool isMouseGrab() const;
		virtual void setMouseGrab(bool);

		virtual bool isShowCursor() const;
		virtual void setShowCursor(bool);

		virtual bool isSystemCursor() const;
		virtual void setSystemCursor(bool state);

		virtual const char *getCursor() const;
		virtual void setCursor(const char *file_name);

		virtual bool isMouseClip() const;
		virtual void setMouseClip(bool state);

		virtual int getMouseX() const;
		virtual int getMouseY() const;
		virtual int getMouseDeltaX() const;
		virtual int getMouseDeltaY() const;
		virtual int getMouseDeltaZ() const;
		virtual bool buttonHold(int button) const;

		//----------------------------------------------------------------------
		// window
		//----------------------------------------------------------------------

		virtual const char *getTitle() const;
		virtual void setTitle(const char *);

		virtual void message(const char *, const char *) const;

		//----------------------------------------------------------------------
		//	clipboard
		//----------------------------------------------------------------------

		virtual const char *getClipboard() const;
		virtual void setClipboard(const char *) const;

		//----------------------------------------------------------------------
		//	common
		//----------------------------------------------------------------------

		virtual void halt();
		virtual bool isActive();
		virtual bool isStarted();
		virtual void swap();

	private:
		void init();
		void done();
		bool loop();
		void input();
	};

    bool PlatformSDL::isStarted() {
		return started;
	}

    bool PlatformSDL::isActive () {
        return (SDL_GetAppState () & SDL_APPACTIVE) == SDL_APPACTIVE;
    }

    bool PlatformSDL::keyHold(int key) const {
        Uint8* keystate = ::SDL_GetKeyState (NULL);
        // Let's assume we are using SDLK_xxx key for now
        // this is not compatible with PlatformWindows which uses scancodes
        // TODO: map scancodes into SDLK_keys or better forget about it and throw PlatformWindows under the bus
		return keystate[key] != 0;
	}

    const char *PlatformSDL::getKeyName(int key) const {
        // TODO: implement later
		return NULL;
	}

	int PlatformSDL::getKeyByName(const char *name) const {
        // TODO: implement later
		return -1;
	}

    const char *PlatformSDL::getClipboard() const {
	// TODO: implement later
	return NULL;
    }

    void PlatformSDL::setClipboard(const char *str) const {
	// TODO: implement later
    }

    const char *PlatformSDL::getCursor() const {
	return cursor.getName();
    }

    int PlatformSDL::getMouseX() const {
	int x = 0; 
	::SDL_GetMouseState (&x, NULL); 
	return x;
    }

    int PlatformSDL::getMouseY() const {
	int y = 0; 
	::SDL_GetMouseState (NULL, &y); 
	return y; 
    }

    void PlatformSDL::setCursor(const char *file_name) {
	cursor.load(file_name);
    }

    int PlatformSDL::getMouseDeltaX() const {
	// TODO: implement later
	return 0;
    }

    int PlatformSDL::getMouseDeltaY() const {
	// TODO: implement later
	return 0;
    }

    int PlatformSDL::getMouseDeltaZ() const {
	// TODO: implement later
	return 0;
    }

    bool PlatformSDL::isMouseClip() const {
	// TODO: implement later
	return false;
    }

    void PlatformSDL::setMouseGrab(bool state) {
	// TODO: implement later
    }

    bool PlatformSDL::isMouseGrab() const {
	// TODO: implement later
	return false;
    }

    bool PlatformSDL::isShowCursor() const {
	// TODO: implement later
	return true;
    }

    void PlatformSDL::setShowCursor(bool state) {
	// TODO: implement later
    }

    bool PlatformSDL::isSystemCursor() const {
	// TODO: implement later
	return true;
    }

    void PlatformSDL::setSystemCursor(bool state) {
	// TODO: implement later
    }

    void PlatformSDL::setMouseClip(bool state) {
	// TODO: implement later
    }
    
    bool PlatformSDL::buttonHold(int button) const {
	if (button < 0 || button >= BUTTON_COUNT) return false;
        
	Uint8 buttons = ::SDL_GetMouseState (0, 0);
        int sdl_button = 0; 
        switch (button) {
        case BUTTON_LEFT:
            sdl_button = 1;
            break;  
        case BUTTON_RIGHT:
            sdl_button = 3;
            break;
        case BUTTON_MIDDLE:
            sdl_button = 2;
            break;
	}

	return (buttons & SDL_BUTTON (sdl_button)) != 0;
    }
    
    int PlatformSDL::main(Application *application_, int argc, const char **argv) {
	try {

	    application = application_;
	    assert(application != NULL);

	    init();

	    started = true;
	    application->onStartup(argc, argv);
	    application->onResize(getWidth(), getHeight());

	    float restDt = 0.0f;

	    application->onUpdate(0.0f);
	    while (true) {
		if (!loop()) break;
		fps.start();

		updateTimer.start();
		if (isActive()) {
		    if (updateFPS <= 1.0f) {
			input();
			application->onUpdate(fps.dt);
		    } else {
			float updateTime = fps.dt + restDt;
			while (updateTime >= 1.0f / updateFPS) {
			    updateTime = updateTime - 1.0f / updateFPS;
			    input();
			    application->onUpdate(1.0f / updateFPS);
			    if (updateFPS <= 1.0f) break;
			}
			restDt = updateTime;
		    }
		}
		updateTimer.finish();

		renderTimer.start();
		application->onRender();
		swap();
		renderTimer.finish();

		fps.finish();
	    }
	    Log::message("* DONE ------------------------------------------------\n");
	} catch (const char *str) {
	    // EVENT(onError(str));
	    done();
	    message(str, "Error");
	    return 1;
	} catch (...) {
	    // EVENT(onError("unknown"));
	    done();
	    message("Unknown", "Error");
	    return 1;
	}

	done();

	return 0;
    }

    float PlatformSDL::getFPS() const {
        return fps.afps;
    }

    void PlatformSDL::clearTimer() const {
    }

    float PlatformSDL::getRenderTime() const {
	return renderTimer.adt * 1000.0f;
    }

    float PlatformSDL::getUpdateTime() const {
	return updateTimer.adt * 1000.0f;
    }

    float PlatformSDL::getFrameTime() const {
	return fps.adt * 1000.0f;
    }

    float PlatformSDL::getUpdateFPS() {
        return updateFPS;
    }

    void PlatformSDL::setUpdateFPS(float fps) {
        updateFPS = (fps < 1.0f) ? 1.0f : fps;
    }

    float PlatformSDL::getDt() const {
        return fps.dt;
    }

    float PlatformSDL::getTimer() {
        return static_cast<float>(SDL_GetTicks()) * 1000.0f;
    }

    void PlatformSDL::done() {
	if (!initialized)
	    return;
        
	application->onShutdown();
	SDL_Quit (); 
	initialized = false;
    }

    void PlatformSDL::init() {
	done();

        // TODO: check for errors
        SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER); 

	setVideoMode(vm_, false);
	initialized = true;
    }

    bool PlatformSDL::setVideoMode(const VideoMode &vm, bool full) {
        vm_          = vm;
        full_screen_ = full;
        
        Uint32 flags = SDL_OPENGL;
        if (full) {
            flags |= SDL_FULLSCREEN;
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);
 
        SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
        
        screen_ = SDL_SetVideoMode (vm.width, vm.height, vm.bits, flags);
	return true;
    }

    int PlatformSDL::getVideoModesCount() const {
        return 1;
    }

    VideoMode PlatformSDL::getVideoMode(int) const {
        return vm_;
    }

    VideoMode PlatformSDL::getCurrentVideoMode() const {
        return vm_;
    }

    bool PlatformSDL::isFullscreen() const {
        return full_screen_;
    }

    int PlatformSDL::getWidth() const {
        return vm_.width;
    }

    int PlatformSDL::getHeight() const {
        return vm_.height;
    } 

    float PlatformSDL::getAspectRatio() const {
        if (getHeight() == 0) {
            return 1.0f;
        }

        return float(getWidth()) / float(getHeight());
    }

    void PlatformSDL::setOritationMode(VideoOrientationType omt) {
    }

    VideoOrientationType PlatformSDL::getOritationMode() const {
        return VIDEO_ORIENTATION_NONE;
    }

    void PlatformSDL::message(const char *message, const char *caption) const {
        // TODO: think of something more elegant
        // just print it out for now
	printf ("%s: %s\n", message, caption);
	}

    void PlatformSDL::setTitle(const char *title) {
        ::SDL_WM_SetCaption (title, 0); 
	}

    // hmm - caller should know not to free/delete our str  
    const char *PlatformSDL::getTitle() const {
	static char str[0x100];
        ::SDL_WM_GetCaption ((char **) (&str), 0); 
	return str;
    }

    void PlatformSDL::halt() {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }

    bool PlatformSDL::loop() {
        SDL_Event event; 

        bool result = true; 
        while (::SDL_PollEvent (&event)) {
	    switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
		    if (application && initialized)
			application->onMouseDown(event.button.x, event.button.y, event.button.button);
		    break;
		case SDL_KEYDOWN:
		    if (application && initialized)
			application->onKeyDown(event.key.keysym.scancode);

		    break;
		case SDL_QUIT: 
		    result = false;
		    break;
	    }
	}

	return result;
    }

    void PlatformSDL::input() {
        // nothing here so far - pumping events should be enough to get access to internal SDL input state
    }

    void PlatformSDL::swap() {
        SDL_GL_SwapBuffers (); 
    }

    void PlatformSDL::onRender() {
//	if (cursor_visible == true && system_cursor == false) {
	    int mx = getMouseX();
	    int my = getMouseY();

	    float cursor_x = mx - float(cursor->getWidth()) / 2.0f;
	    float cursor_y = my - float(cursor->getHeight()) / 2.0f;

	    render->drawImage(cursor_x, cursor_y, cursor);
//	}
    }

    void PlatformSDL::onStartup(int argc, const char **argv) {
//	console->addMember("halt", &PlatformWindows::halt, this);
//	console->addMember("video", &PlatformWindows::commandVideo, this);
//	console->addMember("title", &PlatformWindows::setTitle, this);
	
	setCursor("Core/Textures/cursor.png");
//	updateCursor();
    }
}

#ifndef NUTMEG_BUILD_DLL

#ifdef __cplusplus
extern "C"
#endif    
    int main(int argc, char *argv[]) {
	    return Nutmeg::engineMain(0, NULL);
    }

#endif // NUTMEG_BUILD_DLL

#endif // NUTMEG_PLATFORM_SDL 

namespace Nutmeg {

	#ifdef NUTMEG_PLATFORM_SDL

		bool isPlatformSDLSupported() {
			return true;
		}

		AbstractPlatform *createPlatformSDL(Engine *engine) {
			return new PlatformSDL(engine);
		}

	#else

		bool isPlatformSDLSupported() {
			return false;
		}

		AbstractPlatform *createPlatformSDL(Engine *engine) {
			return NULL;
		}

	#endif
}

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
#include "PlatformWindows.h"
#include "PlatformSDL.h"
#include "AudioSquall.h"

//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------

	void collide(PhysicsBody &a, PhysicsBody &b, CollisionInfo &info, void *data);

	//--------------------------------------------------------------------------

	class MyApp : public Application {

		TextureRef texture;
		TextureRef checker;
		SoundRef bump;
		FontRef font;
		Scene *scene;

		float x;
		float y;
		float dx;
		float dy;

		Camera camera;

	public:

		MyApp() {
			x = 0.0f;
			y = 0.0f;
			dx = 300.0f;
			dy = 300.0f;
			scene = NULL;
		}

		virtual void onStartup(int argc, const char **argv) {

			Log::open("template.log");

			font.load("Core/Fonts/ms_sans_serif_8.xml_font");
			font->bind();

			// platform->setVideoMode(VideoMode(1024, 768, 32), false);

			bump.load("hit1.wav");
			texture.load("fire.png");
			checker.load("Core/Editor/aplha2.jpg");

			// scene
			scene = new Scene(engine);
			scene->init("Newton");
			scene->load("compound.xml_scene");
			scene->getPhysicsWorld().setCollisionCallback(collide, this);

			camera.pos = vec3(0.5f, 0.5f, 0.5f);
			camera.distance = 10.0f;
			camera.min_distance = 20.0f;
			camera.max_distance = 20.0f;
			camera.ang.x = -60;

			platform->setSystemCursor(false);
			//use_fake_aa = true;

		}

		void onShutdown() {
			delete scene;
			scene = NULL;
		}

		void playSound(float volume) {
			bump->setVolume(volume);
			bump->play();
		}

		void onUpdate(float dt) {

			/*
			x += dx * dt;
			y += dy * dt;

			if (x > float(render->getWidth() - texture->getWidth())) {
				x = float(render->getWidth() - texture->getWidth());
				dx = -dx;
				bump->play();
			}

			if (y > float(render->getHeight() - texture->getHeight())) {
				y = float(render->getHeight() - texture->getHeight());
				dy = -dy;
				bump->play();
			}

			if (x < 0.0f) {
				x = 0.0f;
				dx = -dx;
				bump->play();
			}

			if (y < 0.0f) {
				y = 0.0f;
				dy = -dy;
				bump->play();
			}
			*/
			camera.ang.z -= 30.0f * dt;
			camera.update(vec3());

			platform->setTitle(format("fps: %.2f", platform->getFPS()));

			scene->updatePhysics(dt);
			scene->update(dt);

		}

		void onRender() {

			render->begin();

			render->setColor(vec3(1.0f, 1.0f, 1.0f), 1.0f);
			render->clear(true, true);

			/*
			render->begin2d(-1, -1);
			render->drawImageRepeated(0, 0, render->getWidth(), render->getHeight(), checker);
			render->end2d();
            */

			camera.setView(render);
			scene->render(camera, render);


			font->bind();
			render->begin2d(-1, -1);
			render->drawText2d(10, 10, "this is Nutmeg::Core template application");
			render->end2d();



			render->end();
		}

		void onKeyDown(int key) {
			if (key == KEY_ENTER) {
            	scene->restore();
			}
		}


	};

	//--------------------------------------------------------------------------

	void collide(PhysicsBody &a, PhysicsBody &b, CollisionInfo &info, void *data) {
		MyApp *my_app = (MyApp *)data;

		//float speed = (a.getVelocity() - b.getVelocity()).length();
		//float speed = info.velocity.length();
		vec3 v0 = a.getVelocity();
		float r0 = v0.length() * dot(normalize(info.normal), normalize(v0));

		vec3 v1 = b.getVelocity();
		float r1 = v1.length() * dot(normalize(info.normal), normalize(v1));


		float speed = math::abs(r1 - r0);

		if (speed > 0.7f) {
			my_app->playSound(75.0f + speed * 8.0f);
			//my_app->playSound(100.0f);
		}
	}

	//--------------------------------------------------------------------------

	int engineMain(int argc, const char **argv) {

		// if (isPlatformWindowsSupported() == true) {
			// AbstractPlatform::addImplementation("Windows", createPlatformWindows);
		// }

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

		MyApp *application = new MyApp();

		return engine.execute(application, argc, argv);

	}

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------


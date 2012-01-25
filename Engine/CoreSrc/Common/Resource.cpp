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

#include "Resource.h"

//------------------------------------------------------------------------------

#include "AbstractRender.h"
#include "AbstractAudio.h"
#include "Scene.h"
#include "Console.h"

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------

	/*
	template <> ResourceMan <RenderTexture>  *TextureRef::manager = NULL;
	template <> MeshMan *MeshRef::manager = NULL;
	template <> FontMan *FontRef::manager = NULL;
	template <> SkeletonMan *SkeletonRef::manager = NULL;
	template <> AnimationSequenceMan *AnimationSequenceRef::manager = NULL;
	template <> SoundMan *SoundRef::manager = NULL;
	*/

	template <> ResourceMan <RenderTexture> *TextureRef::manager = NULL;
	template <> ResourceMan <RenderMesh> *MeshRef::manager = NULL;
	template <> ResourceMan <RenderFont> *FontRef::manager = NULL;
	template <> ResourceMan <Skeleton> *SkeletonRef::manager = NULL;
	template <> ResourceMan <AnimationSequence> *AnimationSequenceRef::manager = NULL;
	template <> ResourceMan <Sound> *SoundRef::manager = NULL;
	template <> ResourceMan <Scene> *SceneRef::manager = NULL;

	//--------------------------------------------------------------------------
	//
	// ResourceManager
	//
	//--------------------------------------------------------------------------

	ResourceManager::ResourceManager(Engine *engine) : Subsystem <ResourceManager> (engine) {

		texture_man = NULL;
		mesh_man = NULL;
		font_man = NULL;
		sound_man = NULL;
		skeleton_man = NULL;
		animation_sequence_man = NULL;
		scene_man = NULL;

		reload_changed = true;
		reload_period = 5.0f;
		time_to_reload = reload_period;

		remove_unused = false;

		listener = this;
		resource_path = "../Resource/";
		auto_path = false;

	}

	//--------------------------------------------------------------------------

	ResourceManager::~ResourceManager() {

	}

	//--------------------------------------------------------------------------
	// resource events
	//--------------------------------------------------------------------------


	void ResourceManager::onLoadFailed(const char *type, const char *name) {
		console->error("Loading Resource <%s> : can not find \"%s\" file.", type, name);
	}

	//----------------------------------------------------------------------

	void ResourceManager::onLoad(const char *type, const char *name) {
	}

	//----------------------------------------------------------------------

	void ResourceManager::onReload(const char *type, const char *name) {
		console->message(format("Reloading Resource <%s> : \"%s\"", type, name));
	}

	//--------------------------------------------------------------------------
	// reloading
	//--------------------------------------------------------------------------

	bool ResourceManager::isReloadChanged() const {
		return reload_changed;
	}

	//--------------------------------------------------------------------------

	void ResourceManager::setReloadChanged(bool state) {
		reload_changed = state;
		if (reload_changed) {
			time_to_reload = 0.0f;
		}
	}

	//--------------------------------------------------------------------------

	float ResourceManager::getReloadPeriod() const {
		return reload_period;
	}

	//--------------------------------------------------------------------------

	void ResourceManager::setReloadPeriod(float time) {
		reload_period = time;
	}

	//--------------------------------------------------------------------------
	// remove unused
	//--------------------------------------------------------------------------

	bool ResourceManager::isRemoveUnused() const {
		return remove_unused;
	}

	//--------------------------------------------------------------------------

	void ResourceManager::setRemoveUnused(bool state) {
		remove_unused = state;
		texture_man->setRemoveUnused(state);
		mesh_man->setRemoveUnused(state);
		sound_man->setRemoveUnused(state);
		font_man->setRemoveUnused(state);
		skeleton_man->setRemoveUnused(state);
	}

	//--------------------------------------------------------------------------
	// event listener
	//--------------------------------------------------------------------------

	void ResourceManager::setEventListener(ResourceEventListener *l) {
		listener = l;
	}

	//--------------------------------------------------------------------------

	ResourceEventListener *ResourceManager::getEventListener() {
		if (listener == NULL) return this;
		return listener;
	}

	//--------------------------------------------------------------------------
	// paths
	//--------------------------------------------------------------------------

	const char *ResourceManager::getResourcePath() {
		return resource_path.str();
	}

	//--------------------------------------------------------------------------

	void ResourceManager::setResourcePath(const char *path) {
		resource_path = path;
	}

	//--------------------------------------------------------------------------

	bool ResourceManager::isAutoPath() {
		return auto_path;
	}

	//--------------------------------------------------------------------------

	void ResourceManager::setAutoPath(bool value) {
		auto_path = value;
	}

	//--------------------------------------------------------------------------
	// system events
	//--------------------------------------------------------------------------

	void ResourceManager::onStartup(int argc, const char **argv) {

		//----------------------------------------------------------------------
		// texture manager
		//----------------------------------------------------------------------

		RenderTexture *texture = render->loadTexture(Str(getResourcePath()) + "Core/Textures/Checker.png");
		texture_man = new ResourceMan <RenderTexture> (this, texture);
		TextureRef::setManager(texture_man);

		//----------------------------------------------------------------------
		// mesh manager
		//----------------------------------------------------------------------

		RenderMesh *mesh = render->loadMesh(Str(getResourcePath()) + "Core/Primitives/pBox.mesh");
		mesh_man = new ResourceMan <RenderMesh> (this, mesh);
		MeshRef::setManager(mesh_man);

		//----------------------------------------------------------------------
		// font manager
		//----------------------------------------------------------------------

		//RenderFont *font = render->loadFont(Str(getResourcePath()) + "Core/Fonts/dos.xml_font");
		RenderFont *font = render->loadFont(Str(getResourcePath()) + "Core/Fonts/fixed_sys.xml_font");
		font_man = new ResourceMan <RenderFont> (this, font);
		FontRef::setManager(font_man);

		//----------------------------------------------------------------------
		// sound manager
		//----------------------------------------------------------------------

		Sound *sound = audio->loadSound(Str(getResourcePath()) + "Core/Sounds/Silence.wav");
		sound_man = new ResourceMan <Sound> (this, sound);
		SoundRef::setManager(sound_man);

		//----------------------------------------------------------------------
		// skeleton manager
		//----------------------------------------------------------------------

		Skeleton *skeleton = new Skeleton();
		skeleton_man = new ResourceMan <Skeleton> (this, skeleton);
		SkeletonRef::setManager(skeleton_man);

		//----------------------------------------------------------------------
		// animation_sequence manager
		//----------------------------------------------------------------------

		AnimationSequence *animation_sequence = new AnimationSequence();
		animation_sequence_man = new ResourceMan <AnimationSequence> (this, animation_sequence);
		AnimationSequenceRef::setManager(animation_sequence_man);

		//----------------------------------------------------------------------
		// scene manager
		//----------------------------------------------------------------------

		Scene *scene = new Scene(engine);
		scene->init("Newton");
		scene_man = new ResourceMan <Scene> (this, scene);
		SceneRef::setManager(scene_man);

		//----------------------------------------------------------------------

		setRemoveUnused(true);

		//----------------------------------------------------------------------

	}

	//--------------------------------------------------------------------------

	void ResourceManager::onUpdate(float dt) {

		if (reload_changed) {

			time_to_reload -= dt;

			if (time_to_reload <= 0.0f) {
				texture_man->reload(true);
				mesh_man->reload(true);
				sound_man->reload(true);
				font_man->reload(true);
				skeleton_man->reload(true);
				//animation_sequence_man->reload(true);
				//scene_man->reload(true);
				time_to_reload = reload_period;
			}

		}

	}

	//--------------------------------------------------------------------------

	void ResourceManager::onShutdown() {

		delete scene_man;
		scene_man = NULL;
		SceneRef::setManager(NULL);

		delete animation_sequence_man;
		animation_sequence_man = NULL;
		AnimationSequenceRef::setManager(NULL);

		delete skeleton_man;
		skeleton_man = NULL;
		SkeletonRef::setManager(NULL);

		delete sound_man;
		sound_man = NULL;
		SoundRef::setManager(NULL);

		delete font_man;
		font_man = NULL;
		FontRef::setManager(NULL);

		delete mesh_man;
		mesh_man = NULL;
		MeshRef::setManager(NULL);

		delete texture_man;
		texture_man = NULL;
		TextureRef::setManager(NULL);

	}

	//--------------------------------------------------------------------------
	// console
	//--------------------------------------------------------------------------

	void ResourceManager::consoleListTextures() {
		console->echo("Textures:");
		for (int i=0; i<texture_man->getItemsCount(); i++) {
			Str name = texture_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListMeshes() {
		console->echo("Meshes:");
		for (int i=0; i<mesh_man->getItemsCount(); i++) {
			Str name = mesh_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListSounds() {
		console->echo("Sounds:");
		for (int i=0; i<sound_man->getItemsCount(); i++) {
			Str name = sound_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListFonts() {
		console->echo("Fonts:");
		for (int i=0; i<font_man->getItemsCount(); i++) {
			Str name = font_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListSkeletons() {
		console->echo("Skeletons:");
		for (int i=0; i<skeleton_man->getItemsCount(); i++) {
			Str name = skeleton_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListAnimations() {
		console->echo("Animation sequences:");
		for (int i=0; i<animation_sequence_man->getItemsCount(); i++) {
			Str name = animation_sequence_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

	void ResourceManager::consoleListScenes() {
		console->echo("Scenes:");
		for (int i=0; i<scene_man->getItemsCount(); i++) {
			Str name = scene_man->getItem(i).name;
			console->echo(format("    %s"), name.str());
		}
	}

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------


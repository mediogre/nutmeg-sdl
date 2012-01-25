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

#ifndef ResourceH
#define ResourceH

//------------------------------------------------------------------------------

#include "Debug.h"
#include "Str.h"
#include "Xml.h"
#include "Log.h"
#include "Map.h"
#include "File.h"
#include "Factory.h"
#include "Timer.h"
#include "Subsystem.h"
#include "LibApi.h"

//#include "AbstractRender.h"

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------

	class Engine;
	class RenderMesh;
	class RenderTexture;
	class RenderFont;
	class Material;
	class Light;
	class Skeleton;
	class AnimationSequence;
	class Sound;
	class Scene;

	//--------------------------------------------------------------------------

	template <class T> class ResourceMan;

	//--------------------------------------------------------------------------
	//
	// class ResourceEventsListener
	//
	//--------------------------------------------------------------------------

	class NUTMEG_API ResourceEventListener {

	public:

		virtual void onLoad(const char *type, const char *name) { }
		virtual void onReload(const char *type, const char *name) { }
		virtual void onLoadFailed(const char *type, const char *name) { }

	};

	//--------------------------------------------------------------------------
	//
	// class ResourceItem
	//
	//--------------------------------------------------------------------------

	template <class T>
	class ResourceItem {

	public:

		Str name;
		T *data;
		int ref_count;

		Str file_name;
		int time_stamp;

		ResourceItem(T *data_) : ref_count(0) {
			data = data_;
			assert(data != NULL);
		}

		~ResourceItem() {
			delete data;
		}

	};

	//--------------------------------------------------------------------------
	//
	// class Resource
	//
	//--------------------------------------------------------------------------

	template <class T>
	class Resource {

		friend class ResourceMan <T>;

		static ResourceMan <T> *manager;
		ResourceItem <T> *resource_item;

		Resource(ResourceItem <T> *resource_item_) : resource_item(NULL) {
			resource_item = resource_item_;
			assert(resource_item != NULL);
			resource_item->ref_count ++;
		}

	public:

		Resource() : resource_item(NULL) {

			if (manager != NULL) {
				resource_item = manager->getDefaultResourceItem();
				resource_item->ref_count ++;
			}

		}

		//
		Resource(const Resource &o) {
			resource_item = o.resource_item;
			resource_item->ref_count ++;
		}

		//
		Resource(const char *name) {
			resource_item = NULL;
			if (manager != NULL) {
				*this = manager->load(name);
			}
		}

		//
		~Resource() {
			if (resource_item == NULL) return;
			resource_item->ref_count --;
			if (manager != NULL) manager->freeUnused();
		}

		bool is() const {
			return resource_item != NULL/* && resource_item->data != manager->getDefaultResource()*/;
		}

		void load(const char *name) {
			*this = manager->load(name);
		}

		Resource <T> &operator=(const Resource <T> &o) {
			if (&o == this) return *this;
			if (resource_item != NULL) resource_item->ref_count --;
			resource_item = o.resource_item;
			resource_item->ref_count ++;
			manager->freeUnused();
			return *this;
		}

		//
		const T *operator->() const {
			if (resource_item == NULL) {
				ResourceItem <T> *&item = const_cast <ResourceItem <T> *&> (resource_item);
				item = manager->getDefaultResourceItem();
				item->ref_count ++;
			}
			return resource_item->data;
		}

		//
		const T &get() const {
			if (resource_item == NULL) {
				ResourceItem <T> *&item = const_cast <ResourceItem <T> *&> (resource_item);
				item = manager->getDefaultResourceItem();
				item->ref_count ++;
			}
			return *resource_item->data;
		}

		//
		operator const T &() const {
			if (resource_item == NULL) {
				ResourceItem <T> *&item = const_cast <ResourceItem <T> *&> (resource_item);
				item = manager->getDefaultResourceItem();
				item->ref_count ++;
			}
			return *resource_item->data;
		};

		bool operator==(const Resource <T> &o) const {
			return resource_item == o.resource_item;
		}

		//
		const char *getName() const {
			if (resource_item == NULL) {
				ResourceItem <T> *&item = const_cast <ResourceItem <T> *&> (resource_item);
				item = manager->getDefaultResourceItem();
				item->ref_count ++;
			}
			return resource_item->name;
		}

		//
		void writeXml(const char *name, Xml *xml) const {
			if (resource_item == NULL) {
				ResourceItem <T> *&item = const_cast <ResourceItem <T> *&> (resource_item);
				item = manager->getDefaultResourceItem();
				item->ref_count ++;
			}
			xml->setChildData(name, resource_item->file_name);
		}

		void readXml(const char *name, const Xml *xml) {
			Str file;
			xml->getChildData(name, file);
			load(file);
		}

		static ResourceMan <T> *getManager() {
			return manager;
		}

		static void setManager(ResourceMan <T> *man) {
			manager = man;
		}

		void setDefault() {
			*this = manager->load(manager->getDefaultResourceItem()->name);
		}

	};

	//--------------------------------------------------------------------------

	/*
	class TextureMan;
	class MeshMan;
	class SoundMan;
	class FontMan;
	class SkeletonMan;
	class AnimationSequenceMan;
	class SceneMan;
	*/

	//--------------------------------------------------------------------------
	//
	// class ResourceManager
	//
	//--------------------------------------------------------------------------

	class ResourceManager : public Subsystem <ResourceManager>, public ResourceEventListener {

		//----------------------------------------------------------------------

		ResourceMan <RenderTexture> *texture_man;
		ResourceMan <RenderMesh> *mesh_man;
		ResourceMan <Sound> *sound_man;
		ResourceMan <RenderFont> *font_man;
		ResourceMan <Skeleton> *skeleton_man;
		ResourceMan <AnimationSequence> *animation_sequence_man;
		ResourceMan <Scene> *scene_man;

		bool reload_changed;
		float time_to_reload;
		float reload_period;

		bool remove_unused;

		ResourceEventListener *listener;
		Str resource_path;
		bool auto_path;

		//----------------------------------------------------------------------
		// console
		//----------------------------------------------------------------------

		void consoleListTextures();
		void consoleListMeshes();
		void consoleListSounds();
		void consoleListFonts();
		void consoleListSkeletons();
		void consoleListAnimations();
		void consoleListScenes();

		//----------------------------------------------------------------------

	public:

		//----------------------------------------------------------------------

		ResourceManager(Engine *engine);
		virtual ~ResourceManager();

		//----------------------------------------------------------------------
		// events
		//----------------------------------------------------------------------

		virtual void onStartup(int argc, const char **argv);
		virtual void onUpdate(float dt);
		virtual void onShutdown();

		//----------------------------------------------------------------------
		// reloading
		//----------------------------------------------------------------------

		bool isReloadChanged() const;
		void setReloadChanged(bool state);

		float getReloadPeriod() const;
		void setReloadPeriod(float time);

		//----------------------------------------------------------------------
		// remove unused
		//----------------------------------------------------------------------

		bool isRemoveUnused() const;
		void setRemoveUnused(bool state);

		//----------------------------------------------------------------------
		// event listener
		//----------------------------------------------------------------------

		ResourceEventListener *getEventListener();
		void setEventListener(ResourceEventListener *listener);

		virtual void onLoad(const char *type, const char *name);
		virtual void onReload(const char *type, const char *name);
		virtual void onLoadFailed(const char *type, const char *name);

		//----------------------------------------------------------------------
		// paths
		//----------------------------------------------------------------------

		const char *getResourcePath();
		void setResourcePath(const char *path);

		bool isAutoPath();
		void setAutoPath(bool state);

		//----------------------------------------------------------------------

	};

	//--------------------------------------------------------------------------
	//
	// class ResourceMan
	//
	//--------------------------------------------------------------------------

	template <class T>
	class ResourceMan {

		ResourceManager *resource_manager;
		Map <Str, ResourceItem <T> > items;
		T *default_resource;
		ResourceItem <T> *default_item;
		bool free_unused;

		ResourceItem <T> *getItemByName(const char *id) {
			return items.get(id);
		}

		bool debug_log;
		bool freeing;

	public:

		//
		ResourceMan(ResourceManager *resource_manager_, T *def_res) {

			resource_manager = resource_manager_;
			assert(resource_manager != NULL);

			default_resource = def_res;
			assert(default_resource != NULL);

			default_item = new ResourceItem <T>(default_resource);
			default_item->name = typeid(T).name();
			items.append(new Str(default_item->name), default_item);

			free_unused = true;
			freeing = false;
			debug_log = false;
		}

		virtual ~ResourceMan() {
		}

		Resource <T> load(const char *name_) {

			Str name = name_;

			ResourceItem <T> *item = getItemByName(name);
			if (item == NULL) {

				Str find = "";

				if (name == "") return default_item;

				if (resource_manager->isAutoPath() == true) {
					find = extractFileName(name);
					find = findFile(find, resource_manager->getResourcePath());
				} else {
					find = Str(resource_manager->getResourcePath()) + name;
					File file(find, "rb");
					if (file.opened() == false) {
						find = "";
					}
				}

				if (find == "") {
					resource_manager->getEventListener()->onLoadFailed(typeid(T).name(), name.str());
					return default_item;
				}

				//Log::message("Loading Resource <%18s> (%16s)...\n", typeid(T).name(), find.str());
				resource_manager->getEventListener()->onLoad(typeid(T).name(), find.str());

				Timer timer;
				timer.start();

				item = new ResourceItem <T> (default_resource->loadResource(find));
				item->file_name = find;
				item->name = name;

				File file(find, "rb");
				item->time_stamp = file.getTime();
				file.close();
				//Log::message("%.2f ms\n", timer.elapsed() * 1000.0f);
				items.append(new Str(name), item);

			}

			return Resource <T> (item);
		}

		Resource <T> create(const char *name, T *resource) {
			ResourceItem <T> *item = items.get(name);

			if (item == NULL) {
				item = new ResourceItem <T> (resource);
				item->file_name = name;
				item->name = name;
				item->data = resource;
				items.append(new Str(item->name), item);
			}

			return Resource <T> (item);
		}

		int getItemsCount() const {
			return items.count();
		}

		const ResourceItem <T> &getItem(int index) const {
			return items[index].getValue();
		}

		const ResourceItem <T> *getItemByName(const char *id) const {
			return items.get(id);
		}

		T *getDefaultResource() {
			return default_resource;
		}

		ResourceItem <T> *getDefaultResourceItem() {
			return default_item;
		}

		void freeUnused() {
			if (free_unused == false) return;
			if (freeing == true) return;

			bool found = true;

			freeing = true;
			while (found) {
				found = false;
				for (int i=1; i<items.count(); i++) {
					if (items[i].getValue().ref_count == 0) {
						items.remove(i--);
						found = true;
						break;
					}
				}
			}
			freeing = false;

		}

		int getItemsCount() {
			return items.count();
		}

		 const ResourceItem <T> &getItem(int i) {
			return items[i].getValue();
		}

		bool reload(bool changed = false) {
			bool ret = false;
			for (int i=1; i<getItemsCount(); i++) {
				bool reload = !changed;
				int new_time;
				if (!reload) {
					File file(getItem(i).file_name, "rb");
					new_time = file.getTime();
					reload = new_time != getItem(i).time_stamp;
				}
				if (reload) {
					resource_manager->getEventListener()->onReload(typeid(T).name(), getItem(i).name);
					items[i].getValue().data->load(getItem(i).file_name);
					items[i].getValue().time_stamp = new_time;
					ret = true;
				}
			}
			return ret;
		}

		bool isRemoveUnused() {
			return free_unused;
		}

		void setRemoveUnused(bool state) {
			free_unused = state;
			if (free_unused) freeUnused();
		}

		bool isDebugLog() const {
			return debug_log;
		}

		void setDebugLog(bool state) {
			debug_log = state;
		}

	};

	//--------------------------------------------------------------------------

	typedef Resource <RenderMesh> MeshRef;
	//class MeshMan : public ResourceMan <RenderMesh> { };

	typedef Resource <RenderTexture> TextureRef;
	//class TextureMan : public ResourceMan <RenderTexture> { };

	typedef Resource <RenderFont> FontRef;
	//class FontMan : public ResourceMan <RenderFont> { };

	typedef Resource <Skeleton> SkeletonRef;
	//class SkeletonMan : public ResourceMan <Skeleton> { };

	typedef Resource <AnimationSequence> AnimationSequenceRef;
	//class AnimationSequenceMan : public ResourceMan <AnimationSequence> { };

	typedef Resource <Sound> SoundRef;
	//class SoundMan : public ResourceMan <Sound> { };

	typedef Resource <Scene> SceneRef;
	//class SceneMan : public ResourceMan <Scene> { };

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------


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

#ifndef ParticleNodeH
#define ParticleNodeH

//------------------------------------------------------------------------------

#include "Node.h"
#include "LibApi.h"

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------
	//
	// class ParticleNode
	//
	//--------------------------------------------------------------------------
	
	class NUTMEG_API ParticleNode : public Node {

		void invalidate();

		float size;
		bool constSize;

		ParticleNode(const ParticleNode &) : Node(NULL)  { }

	protected:

		ParticleNode &operator= (const ParticleNode &);

	public:

		//----------------------------------------------------------------------

		ParticleNode(Scene *scene);

		//----------------------------------------------------------------------

		static NodeType getStaticType();

		//----------------------------------------------------------------------
		
		virtual void render(AbstractRender *render) const;
		virtual Node *clone(Scene *scene) const;
		virtual bool trace(const Line &line, TracePoint &p, bool fs, bool fd) const;
		virtual void invalidateMatrix();
		virtual void renderHelper(AbstractRender *render, bool selected) const;


		//--------------------------------------------------------------------------

		void setSize(float size);
		float getSize() const;

		void setConstSize(bool size);
		bool getConstSize() const;

		//----------------------------------------------------------------------

		virtual void writeXml(Xml *) const;
		virtual void readXml(const Xml *);

		//----------------------------------------------------------------------

	};

	//--------------------------------------------------------------------------

}

#endif

//------------------------------------------------------------------------------

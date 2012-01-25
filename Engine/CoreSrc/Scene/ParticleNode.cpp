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

#include "ParticleNode.h"

//------------------------------------------------------------------------------

#include "AbstractRender.h"
#include "Camera.h"

//------------------------------------------------------------------------------
//
// namespace Nutmeg
//
//------------------------------------------------------------------------------

namespace Nutmeg {

	//--------------------------------------------------------------------------

	ParticleNode::ParticleNode(Scene *scene_) : Node(scene_) {
		type = NODE_PARTICLE;
		size = 1.0f;
		constSize = false;
		invalidate();
		setName(getTypeName());
	}

	//--------------------------------------------------------------------------

	NodeType ParticleNode::getStaticType() {
		return NODE_PARTICLE;
	}

	//--------------------------------------------------------------------------

	void ParticleNode::invalidate() {
		box = BBox(vec3(-size / 2, -size / 2, -size / 2), vec3(size / 2, size / 2, size / 2));
	}

	//--------------------------------------------------------------------------
	/*
	static vec3 toCameraPlane(const vec3 &pos, const Camera &c) {
		vec3 prj = c.getFrustum().project(pos);
		Line ray = c.getRay(prj.x, prj.y);
		return c.pos + normalize(ray.dst - ray.src) * 5;
	}
	*/
	//--------------------------------------------------------------------------

	void ParticleNode::render(AbstractRender *render) const {
		render->setMatrix(mat4());
		vec3 p = getPos(true);
		/*
		if (constSize) {
			p = toCameraPlane(p, c);
		}
		*/
		render->drawBillboard(p, size * ((constSize) ? 0.01f : 1.0f));
	//	renderer.setMatrix(origin);
	//	renderer.render(box);
	}

	//--------------------------------------------------------------------------

	ParticleNode &ParticleNode::operator= (const ParticleNode &o) {
		Node::operator= (o);
		size = o.size;
		constSize = o.constSize;
		return *this;
	}

	//--------------------------------------------------------------------------

	Node *ParticleNode::clone(Scene *scene) const {
		ParticleNode *node = new ParticleNode(scene);
		*node = *this;
		return node;
	}

	//--------------------------------------------------------------------------

	void ParticleNode::writeXml(Xml *xml) const {

		Node::writeXml(xml);

		xml->setChildData("size", size);
		xml->setChildData("const_size", constSize);

	}

	//--------------------------------------------------------------------------

	void ParticleNode::readXml(const Xml *xml) {

		Node::readXml(xml);

		xml->getChildData("size", size);
		xml->getChildData("const_size", constSize);

		invalidate();
	}

	//--------------------------------------------------------------------------

	void ParticleNode::setSize(float s) {
		size = s;
		invalidate();
	}

	//--------------------------------------------------------------------------

	float ParticleNode::getSize() const {
		return size;
	}

	//--------------------------------------------------------------------------

	void ParticleNode::setConstSize(bool s) {
		constSize = s;
		invalidate();
	}

	//--------------------------------------------------------------------------

	bool ParticleNode::getConstSize() const {
		return constSize;
	}

	//--------------------------------------------------------------------------

	bool ParticleNode::trace(const Line &line, TracePoint &p, bool fs, bool fd) const {
		Line iLine = line;
		iLine.src = inverse * iLine.src;
		iLine.dst = inverse * iLine.dst;
		bool ret = BSphere(vec3(0, 0, 0), 0.1).trace(iLine, p, fs, fd);
		if (ret) {
			p.point = origin * p.point;
			p.normal = normalize(origin * p.normal - origin * vec3(0, 0, 0));
		}
		return ret;
	}

	//--------------------------------------------------------------------------

	void ParticleNode::invalidateMatrix() {
		float factor = 1.0f;
		for (int i=0; i<3; i++) {
			if (scale[i] != 1.0f) {
				factor = scale[i];
				break;
			}
		}

		scale = vec3(1.0f, 1.0f, 1.0f);
		size *= factor;

		Node::invalidateMatrix();
		invalidate();
	}

	//--------------------------------------------------------------------------

	void ParticleNode::renderHelper(AbstractRender *render, bool selected) const {
		render->setMatrix(mat4());
		vec3 pos = getPos(true);
		if (selected) {
			render->drawSphere(BSphere(pos, sphere.radius));
		}
		render->drawSphere(BSphere(pos, 0.1));
	}

	//--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------

//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018, 2019 Orthrus Group.                         |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: SceneObject.h
// ========
// Class definition for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 23/09/2019

#ifndef __SceneObject_h
#define __SceneObject_h

#include "SceneNode.h"
#include "Transform.h"
#include "Primitive.h"
#include "Light.h"
#include <vector>
#include <algorithm>

namespace cg
{ // begin namespace cg

// Forward definition
	class Scene;


/////////////////////////////////////////////////////////////////////
//
// SceneObject: scene object class
// ===========
class SceneObject : public SceneNode
{
public:
	bool visible{ true };

	/// Constructs an empty scene object.
	SceneObject(const char* name, Scene& scene) :
		SceneNode{ name },
		_scene{ &scene },
		_parent{}
	{
		//addComponent(makeUse(&_transform));
	}

	/// Returns the scene which this scene object belong to.
	auto scene() const
	{
		return _scene;
	}

	/// Returns the parent of this scene object.
	auto parent() const
	{
		return _parent;
	}

	/// Sets the parent of this scene object.
	void setParent(SceneObject* parent);

	void parentSet(SceneObject* parent);
	/// Add child to list of childs 
	void addChild(SceneObject* child);

	void removeChild(SceneObject* child);

	int childCount();

	/// Returns the transform of this scene object.
	auto transform() const
	{
		return _transform;
	}

	auto transform()
	{
		return _transform;
	}

	Primitive* primitive()
	{
		return _primitive;
	}

	Light* light()
	{
		return _light;
	}

	void addComponent(Component* comp);


	// **Begin temporary methods
	// They should be replace by your child and component iterators
	std::vector<Reference<SceneObject>>::iterator childBegin()
	{
		return _child.begin();
	}

	std::vector<Reference<SceneObject>>::iterator childEnd()
	{
		return _child.end();
	}

	std::vector<Reference<Component>>::iterator componentBegin()
	{
		return _components.begin();
	}

	std::vector<Reference<Component>>::iterator componentEnd()
	{
		return _components.end();
	}

	int componentCount();

	Component* getComponent(int k);

	void att_components(SceneObject* x);

	SceneObject* getChild(int k);
	// **End temporary methods

private:
	Scene* _scene;
	SceneObject* _parent;
	Transform* _transform{};
	Primitive* _primitive{};
	Light* _light{};
	// **Begin temporary attributes
	// They should be replace by your child and component collections
	//declarada a lista de filhos
	std::vector<Reference<SceneObject>> _child;
	//declara a lista de componentes
	std::vector<Reference<Component>> _components;
	// **End temporary attributes

	friend class Scene;

}; // SceneObject

/// Returns the transform of a component.
inline Transform*
	Component::transform() // declared in Component.h
{
	return sceneObject()->transform();
}

/// Returns the parent of a transform.
inline Transform*
	Transform::parent() const // declared in Transform.h
{
	if (auto p = sceneObject()->parent())
		return p->transform();
	return nullptr;
}

} // end namespace cg

#endif // __SceneObject_h

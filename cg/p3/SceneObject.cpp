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
// OVERVIEW: SceneObject.cpp
// ========
// Source file for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 07/09/2019

#include "SceneObject.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneObject implementation
// ===========
void
SceneObject::setParent(SceneObject* parent)
{
	cg::SceneObject* obj = new cg::SceneObject(name(), *_scene);
	obj->parentSet(parent);
	parent->addChild(obj);
	auto novo = parent->getChild(parent->childCount() - 1);
	SceneObject* aux = _parent;
	_parent = parent;
	_transform.parentChanged();
	_parent = aux;
	for (auto it = _components.begin(); it != _components.end(); it++)
		novo->addComponent(&(**it));
	novo->att_components(this);
}
void SceneObject::parentSet(SceneObject* parent)
{
	_parent = parent;
}

void
SceneObject::att_components(SceneObject* x)
{
	_child.clear();
	for (std::vector<Reference<SceneObject>>::iterator it = x->childBegin(); it != x->childEnd(); ++it)
	{
		this->addChild(&(**it));
		SceneObject* aux = this->getChild(this->childCount() - 1);
		aux->parentSet(this);
		for (auto component = aux->componentBegin(); component != aux->componentEnd(); component++)
			(*component)->_sceneObject = aux;
		aux->att_components(&(**it));
	}
}

void
SceneObject::addChild(SceneObject* child)
{
	_child.push_back(child);
}

void
SceneObject::removeChild(SceneObject* child)
{
	auto size = _child.size();
	unsigned index = -1;
	for (unsigned i = 0; i < size; ++i)
	{
		if ((SceneObject*)_child[i] == child)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
		_child.erase(_child.begin() + index);
}

void
SceneObject::addComponent(Component* comp)
{
	bool flag = true;
	for (auto it = _components.begin(); it != _components.end(); it++)
		if (strcmp((*it)->typeName(), comp->typeName()) == 0)
			flag = false;
	if (flag)
	{
		comp->_sceneObject = this;
		_components.push_back(comp);
		/*if (strcmp("Transform", comp->typeName()) == 0)
			_transform = (Transform*) & (**--_components.end());*/
		/*else*/ if (strcmp("Primitive", comp->typeName()) == 0)
			_primitive = (Primitive*) & (**--_components.end());
		else if (strcmp("Light", comp->typeName()) == 0)
			_light = (Light*) & (**--_components.end());
	}
}

SceneObject* SceneObject::getChild(int k)
{
	if (k >= _child.size())
		printf("Error: Object has no child numbered %d", k);
	return _child.at(k);
}

int SceneObject::childCount()
{
	return _child.size();
}

int SceneObject::componentCount()
{
	return _components.size();
}

Component* SceneObject::getComponent(int k)
{
	return _components[k];
}
// end namespace cg
} // end namespace cg

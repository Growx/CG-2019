#include "geometry/MeshSweeper.h"
#include "P3.h"
#include <string>
#include <stack>

MeshMap P3::_defaultMeshes;

inline void
P3::buildDefaultMeshes()
{
  _defaultMeshes["None"] = nullptr;
  _defaultMeshes["Box"] = GLGraphics3::box();
  _defaultMeshes["Sphere"] = GLGraphics3::sphere();
}

inline Primitive*
makePrimitive(MeshMapIterator mit)
{
  return new Primitive(mit->second, mit->first);
}

inline void
P3::buildScene()
{
  _current = _scene = new Scene{"Scene 1"};
  _editor = new SceneEditor{*_scene};
  _editor->setDefaultView((float)width() / (float)height());
  // **Begin initialization of temporary attributes
   //It should be replaced by your scene initialization
  SceneObject* box = new SceneObject("Box_0", *_scene);
  box->addComponent(new Transform());
  box->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  box->setParent(_root);

  box = new SceneObject("Box_1", *_scene);
  box->addComponent(new Transform());
  box->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  box->setParent(_root);
  //box->transform()->setPosition(vec3f(4, 0, 0));

  box = new SceneObject("Camera_0", *_scene);
  box->addComponent(new Camera());
  box->setParent(_root);

  SceneObject* l1 = new SceneObject("Gray_directional_light", *_scene);
  l1->addComponent(new Transform());
  l1->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  l1->addComponent(new Light());
  l1->setParent(_root);
  /*l1->transform()->setPosition(vec3f(0, 3, 0));
  l1->transform()->setLocalScale(vec3f(0.1, 0.1, 0.1));*/
  l1->light()->setCor(Color::gray);
  l1->light()->setType(Light::Type::Directional);

  SceneObject* l2 = new SceneObject("Magenta_point_light", *_scene);
  l2->addComponent(new Transform());
  l2->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  l2->addComponent(new Light());
  l2->setParent(_root);
  //l2->transform()->setPosition(vec3f(2, 1, -1));
  //l2->transform()->setLocalScale(0.1);
  l2->light()->setCor(Color::magenta);
  l2->light()->setType(Light::Type::Point);

  SceneObject* l3 = new SceneObject("Cyan_spot_light", *_scene);
  l3->addComponent(new Transform());
  l3->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  l3->addComponent(new Light());
  l3->setParent(_root);
  //l3->transform()->setPosition(vec3f(-1, 2, 1));
  //l3->transform()->setLocalScale(0.1);
  l3->light()->setCor(Color::cyan);
  l3->light()->setType(Light::Type::Spot);

  SceneObject* l4 = new SceneObject("Green_spot_light", *_scene);
  l4->addComponent(new Transform());
  l4->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
  l4->addComponent(new Light());
  l4->setParent(_root);
  //l4->transform()->setPosition(vec3f(4, 3, 0));
  //l4->transform()->setLocalScale(0.1);
  l4->light()->setCor(Color::green);
  l4->light()->setType(Light::Type::Spot);
  // **End initialization of temporary attributes
}

void
P3::initialize()
{
	Application::loadShaders(_program[0], "shaders/p3-none.vs", "shaders/p3-none.fs");
	Application::loadShaders(_program[1], "shaders/p3-flat.vs", "shaders/p3-flat.fs");
	Application::loadShaders(_program[2], "shaders/p3.vs", "shaders/p3.fs");
	Application::loadShaders(_program[3], "shaders/p3-smooth.vs", "shaders/p3-smooth.fs");  
	
	Assets::initialize();
  buildDefaultMeshes();
  buildScene();
  _renderer = new GLRenderer{*_scene};
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  glEnable(GL_LINE_SMOOTH);
  _program[0].use();
  _b = Tonalizacao::None;

}

inline void
P3::hierarchyWindow()
{
	static int j = 0;
	static int i = 0;
	static int k = 0;

  ImGui::Begin("Hierarchy");
  if (ImGui::Button("Create###object"))
    ImGui::OpenPopup("CreateObjectPopup");
  if (ImGui::BeginPopup("CreateObjectPopup"))
  {
    if (ImGui::MenuItem("Empty Object"))
    {
      // TODO: create an empty object.
		std::string s = "Object_" + std::to_string(j);
		SceneObject* obj = new SceneObject(s.c_str(), *_scene);
		obj->addComponent(new Transform());
		obj->setParent(_currentObj);
		printf("%s\n", _currentObj->getChild(_currentObj->childCount() - 1)->name());
		++j;
    }
    if (ImGui::BeginMenu("3D Object"))
    {
      if (ImGui::MenuItem("Box"))
      {
        // TODO: create a new box.
		  std::string s = "Box_" + std::to_string(i);
		  SceneObject* obj = new SceneObject(s.c_str(), *_scene);
		  obj->addComponent(new Transform());
		  obj->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("Box")));
		  obj->setParent(_currentObj);
		  printf("%s\n", _currentObj->getChild(_currentObj->childCount() - 1)->name());
		  ++i;
      }
      if (ImGui::MenuItem("Sphere"))
      {
        // TODO: create a new sphere.
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Light"))
    {
		std::string s = "Light_" + std::to_string(k);
      if (ImGui::MenuItem("Directional Light"))
      {
        // TODO: create a new directional light.

		  auto o = new SceneObject{ "Directional Light", *_scene };
		  SceneObject* obj = new SceneObject((s + "_Dir").c_str(), *_scene);
		  obj->addComponent(new Transform());
		  obj->addComponent(new Light());
		  obj->setParent(_currentObj);
		  obj->light()->setType(Light::Type::Directional);
		  ++k;
      }
      if (ImGui::MenuItem("Point Light"))
      {
        // TODO: create a new pontual light.
		  SceneObject* obj = new SceneObject((s + "_Point").c_str(), *_scene);
		  obj->addComponent(new Transform());
		  obj->addComponent(new Light());
		  obj->setParent(_currentObj);
		  ++k;
      }
      if (ImGui::MenuItem("Spotlight"))
      {
        // TODO: create a new spotlight.
		  SceneObject* obj = new SceneObject((s + "_Spot").c_str(), *_scene);
		  obj->addComponent(new Transform());
		  obj->addComponent(new Light());
		  obj->setParent(_currentObj);
		  obj->light()->setType(Light::Type::Spot);
		  ++k;
      }
      ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Camera"))
    {
      // TODO: create a new camera.
    }
    ImGui::EndPopup();
  }
  ImGui::Separator();

  // **Begin hierarchy of temporary scene objects
  // It should be replaced by your hierarchy
  auto f = ImGuiTreeNodeFlags_OpenOnArrow;
  auto open = ImGui::TreeNodeEx(_scene,
    _current == _scene ? f | ImGuiTreeNodeFlags_Selected : f,
    _scene->name());

  if (ImGui::IsItemClicked())
    _current = _scene;
  if (open)
  {
    for (const auto& o : _objects)
    {
      auto f = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

      ImGui::TreeNodeEx(o,
        _current == o ? f | ImGuiTreeNodeFlags_Selected : f,
        o->name());
      if (ImGui::IsItemClicked())
        _current = o;
    }
    ImGui::TreePop();
  }
  // **End hierarchy of temporary scene objects
  ImGui::End();
}

namespace ImGui
{ // begin namespace ImGui

void
ObjectNameInput(NameableObject* object)
{
  const int bufferSize{128};
  static NameableObject* current;
  static char buffer[bufferSize];

  if (object != current)
  {
    strcpy_s(buffer, bufferSize, object->name());
    current = object;
  }
  if (ImGui::InputText("Name", buffer, bufferSize))
    object->setName(buffer);
}

inline bool
ColorEdit3(const char* label, Color& color)
{
  return ImGui::ColorEdit3(label, (float*)&color);
}

inline bool
DragVec3(const char* label, vec3f& v)
{
  return DragFloat3(label, (float*)&v, 0.1f, 0.0f, 0.0f, "%.2g");
}

void
TransformEdit(Transform* transform)
{
  vec3f temp;

  temp = transform->localPosition();
  if (ImGui::DragVec3("Position", temp))
    transform->setLocalPosition(temp);
  temp = transform->localEulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    transform->setLocalEulerAngles(temp);
  temp = transform->localScale();
  if (ImGui::DragVec3("Scale", temp))
    transform->setLocalScale(temp);
}

} // end namespace ImGui

inline void
P3::sceneGui()
{
  auto scene = (Scene*)_current;

  ImGui::ObjectNameInput(_current);
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorEdit3("Background", scene->backgroundColor);
    ImGui::ColorEdit3("Ambient Light", scene->ambientLight);
  }
}

inline void
P3::inspectShape(Primitive& primitive)
{
  char buffer[16];

  snprintf(buffer, 16, "%s", primitive.meshName());
  ImGui::InputText("Mesh", buffer, 16, ImGuiInputTextFlags_ReadOnly);
  if (ImGui::BeginDragDropTarget())
  {
    if (auto* payload = ImGui::AcceptDragDropPayload("PrimitiveMesh"))
    {
      auto mit = *(MeshMapIterator*)payload->Data;
      primitive.setMesh(mit->second, mit->first);
    }
    ImGui::EndDragDropTarget();
  }
  ImGui::SameLine();
  if (ImGui::Button("...###PrimitiveMesh"))
    ImGui::OpenPopup("PrimitiveMeshPopup");
  if (ImGui::BeginPopup("PrimitiveMeshPopup"))
  {
    auto& meshes = Assets::meshes();

    if (!meshes.empty())
    {
      for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
        if (ImGui::Selectable(mit->first.c_str()))
          primitive.setMesh(Assets::loadMesh(mit), mit->first);
      ImGui::Separator();
    }
    for (auto mit = _defaultMeshes.begin(); mit != _defaultMeshes.end(); ++mit)
      if (ImGui::Selectable(mit->first.c_str()))
        primitive.setMesh(mit->second, mit->first);
    ImGui::EndPopup();
  }
}

inline void
P3::inspectMaterial(Material& material)
{
  ImGui::ColorEdit3("Ambient", material.ambient);
  ImGui::ColorEdit3("Diffuse", material.diffuse);
  ImGui::ColorEdit3("Spot", material.spot);
  ImGui::DragFloat("Shine", &material.shine, 1, 0, 1000.0f);
}

inline void
P3::inspectPrimitive(Primitive& primitive)
{
  //const auto flag = ImGuiTreeNodeFlags_NoTreePushOnOpen;

  //if (ImGui::TreeNodeEx("Shape", flag))
    inspectShape(primitive);
  //if (ImGui::TreeNodeEx("Material", flag))
    inspectMaterial(primitive.material);
}

inline void
P3::inspectLight(Light& light)
{
  static const char* lightTypes[]{"Directional", "Point", "Spot"};
  auto lt = light.type();
  static const char* fatores[]{ "0", "1", "2" };

  if (ImGui::BeginCombo("Type", lightTypes[lt]))
  {
    for (auto i = 0; i < IM_ARRAYSIZE(lightTypes); ++i)
    {
      bool selected = lt == i;

      if (ImGui::Selectable(lightTypes[i], selected))
        lt = (Light::Type)i;
      if (selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  light.setType(lt);
  ImGui::ColorEdit3("Color", light.color);
  Color cor;
  int fator;
  float angulo_interno, angulo_externo;
  cor = light.cor();
  if (ImGui::ColorEdit3("Cor", cor))
	  light.setCor(cor);
  if (lt == Light::Type::Spot)
  {
	  fator = light.fator();
	  if (ImGui::BeginCombo("Fator", fatores[fator]))
	  {
		  for (auto i = 0; i < IM_ARRAYSIZE(fatores); ++i)
		  {
			  bool selected = fator == i;

			  if (ImGui::Selectable(fatores[i], selected))
				  fator = i;
			  if (selected)
				  ImGui::SetItemDefaultFocus();
		  }
		  ImGui::EndCombo();
	  }
	  light.setFator(fator);
	  angulo_interno = light.angulo_interno();
	  angulo_externo = light.angulo_externo();
	  float aux;
	  if (ImGui::DragFloat("Angulo Interno", &angulo_interno, 0.2, 0.1, 89.9))
	  {
		  if (angulo_interno > angulo_externo)
			  aux = angulo_externo - 0.1;
		  else
			  aux = angulo_interno;
		  light.setAngulo_interno(aux);
	  }
	  if (ImGui::DragFloat("Angulo Externo", &angulo_externo, 0.2, 0.1, 89.9))
	  {
		  if (angulo_externo < angulo_interno)
			  aux = angulo_interno + 0.1;
		  else
			  aux = angulo_externo;
		  light.setAngulo_externo(aux);
	  }
  }
  if (lt == Light::Type::Point)
  {
	  fator = light.fator();
	  if (ImGui::BeginCombo("Fator", fatores[fator]))
	  {
		  for (auto i = 0; i < IM_ARRAYSIZE(fatores); ++i)
		  {
			  bool selected = fator == i;

			  if (ImGui::Selectable(fatores[i], selected))
				  fator = i;
			  if (selected)
				  ImGui::SetItemDefaultFocus();
		  }
		  ImGui::EndCombo();
	  }
	  light.setFator(fator);
  }

}

void
P3::inspectCamera(Camera& camera)
{
	static const char* projectionNames[]{ "Perspective", "Orthographic" };
	auto cp = camera.projectionType();

	if (ImGui::BeginCombo("Projection", projectionNames[cp]))
	{
		for (auto i = 0; i < IM_ARRAYSIZE(projectionNames); ++i)
		{
			auto selected = cp == i;

			if (ImGui::Selectable(projectionNames[i], selected))
				cp = (Camera::ProjectionType)i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	camera.setProjectionType(cp);
	if (cp == View3::Perspective)
	{
		auto fov = camera.viewAngle();

		if (ImGui::SliderFloat("View Angle",
			&fov,
			MIN_ANGLE,
			MAX_ANGLE,
			"%.0f deg",
			1.0f))
			camera.setViewAngle(fov <= MIN_ANGLE ? MIN_ANGLE : fov);
	}
	else
	{
		auto h = camera.height();

		if (ImGui::DragFloat("Height",
			&h,
			MIN_HEIGHT * 10.0f,
			MIN_HEIGHT,
			math::Limits<float>::inf()))
			camera.setHeight(h <= 0 ? MIN_HEIGHT : h);
	}

	float n;
	float f;

	camera.clippingPlanes(n, f);

	if (ImGui::DragFloatRange2("Clipping Planes",
		&n,
		&f,
		MIN_DEPTH,
		MIN_DEPTH,
		math::Limits<float>::inf(),
		"Near: %.2f",
		"Far: %.2f"))
	{
		if (n <= 0)
			n = MIN_DEPTH;
		if (f - n < MIN_DEPTH)
			f = n + MIN_DEPTH;
		camera.setClippingPlanes(n, f);
	}
}

inline void
P3::addComponentButton(SceneObject& object)
{
  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("AddComponentPopup");
  if (ImGui::BeginPopup("AddComponentPopup"))
  {
    if (ImGui::MenuItem("Primitive"))
    {
      // TODO
		_currentObj->addComponent((Primitive*)makePrimitive(_defaultMeshes.find("None")));
    }
    if (ImGui::MenuItem("Light"))
    {
      // TODO
		_currentObj->addComponent(new Light());
    }
    if (ImGui::MenuItem("Camera"))
    {
      // TODO
		_currentObj->addComponent(new Camera());

    }
    ImGui::EndPopup();
  }
}

inline void
P3::sceneObjectGui() 
{
	auto object = (SceneObject*)_currentObj;

	addComponentButton(*object);
	ImGui::Separator();
	ImGui::ObjectNameInput(object);
	ImGui::SameLine();
	ImGui::Checkbox("###visible", &object->visible);
	ImGui::Separator();
	if (ImGui::CollapsingHeader(object->transform()->typeName()))
		ImGui::TransformEdit(object->transform());
	// **Begin inspection of temporary components
	// It should be replaced by your component inspection
	if (ImGui::CollapsingHeader(object->transform()->typeName()))
		ImGui::TransformEdit(object->transform());
	if (object->primitive() != nullptr)
		if (ImGui::CollapsingHeader(object->primitive()->typeName()))
			inspectPrimitive(*object->primitive());
	if (object->light() != nullptr)
		if (ImGui::CollapsingHeader(object->light()->typeName()))
			inspectLight(*object->light());
	// **End inspection of temporary components
}

inline void
P3::objectGui()
{
  if (_current == nullptr)
    return;
  if (dynamic_cast<SceneObject*>(_current))
  {
    sceneObjectGui();
    return;
  }
  if (dynamic_cast<Scene*>(_current))
    sceneGui();
}

inline void
P3::inspectorWindow()
{
  ImGui::Begin("Inspector");
  objectGui();
  ImGui::End();
}

inline void
P3::editorViewGui()
{
  if (ImGui::Button("Set Default View"))
    _editor->setDefaultView(float(width()) / float(height()));
  ImGui::Separator();

  auto t = _editor->camera()->transform();
  vec3f temp;

  temp = t->localPosition();
  if (ImGui::DragVec3("Position", temp))
    t->setLocalPosition(temp);
  temp = t->localEulerAngles();
  if (ImGui::DragVec3("Rotation", temp))
    t->setLocalEulerAngles(temp);
  inspectCamera(*_editor->camera());
  ImGui::Separator();
  {
    static int sm;

    ImGui::Combo("Shading Mode", &sm, "None\0Flat\0Gouraud\0\0");
    // TODO
	if (sm == 0)
	{
		if (_b != Tonalizacao::None)
			_program[0].use();
		_b = Tonalizacao::None;
	}
	if (sm == 1)
	{
		if (_b != Tonalizacao::Flat)
			_program[1].use();
		_b = Tonalizacao::Flat;
	}
	if (sm == 2)
	{
		if (_b != Tonalizacao::Gouraud)
			_program[2].use();
		_b = Tonalizacao::Gouraud;
	}
	if (sm == 3)
	{
		if (_b != Tonalizacao::Phong)
			_program[3].use();
		_b = Tonalizacao::Phong;
	}

    static Color edgeColor;
    static bool showEdges;

    ImGui::ColorEdit3("Edges", edgeColor);
	_corArestas = edgeColor;
    ImGui::SameLine();
    ImGui::Checkbox("###showEdges", &showEdges);
  }
  ImGui::Separator();
  ImGui::Checkbox("Show Ground", &_editor->showGround);
}

inline void
P3::assetsWindow()
{
  ImGui::Begin("Assets");
  if (ImGui::CollapsingHeader("Meshes"))
  {
    auto& meshes = Assets::meshes();

    for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
    {
      auto meshName = mit->first.c_str();
      auto selected = false;

      ImGui::Selectable(meshName, &selected);
      if (ImGui::BeginDragDropSource())
      {
        Assets::loadMesh(mit);
        ImGui::Text(meshName);
        ImGui::SetDragDropPayload("PrimitiveMesh", &mit, sizeof(mit));
        ImGui::EndDragDropSource();
      }
    }
  }
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Textures"))
  {
    // next semester
  }
  ImGui::End();
}

inline void
P3::editorView()
{
  if (!_showEditorView)
    return;
  ImGui::Begin("Editor View Settings");
  editorViewGui();
  ImGui::End();
}

inline void
P3::fileMenu()
{
  if (ImGui::MenuItem("New"))
  {
    // TODO
  }
  if (ImGui::MenuItem("Open...", "Ctrl+O"))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Save", "Ctrl+S"))
  {
    // TODO
  }
  if (ImGui::MenuItem("Save As..."))
  {
    // TODO
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Exit", "Alt+F4"))
  {
    shutdown();
  }
}

inline bool
showStyleSelector(const char* label)
{
  static int style = 1;

  if (!ImGui::Combo(label, &style, "Classic\0Dark\0Light\0"))
    return false;
  switch (style)
  {
    case 0: ImGui::StyleColorsClassic();
      break;
    case 1: ImGui::StyleColorsDark();
      break;
    case 2: ImGui::StyleColorsLight();
      break;
  }
  return true;
}

inline void
P3::showOptions()
{
  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
  showStyleSelector("Color Theme##Selector");
  ImGui::ColorEdit3("Selected Wireframe", _selectedWireframeColor);
  ImGui::PopItemWidth();
}

inline void
P3::mainMenu()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      fileMenu();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
      if (Camera::current() == 0)
        ImGui::MenuItem("Edit View", nullptr, true, false);
      else
      {
        static const char* viewLabels[]{"Editor", "Renderer"};

        if (ImGui::BeginCombo("View", viewLabels[_viewMode]))
        {
          for (auto i = 0; i < IM_ARRAYSIZE(viewLabels); ++i)
          {
            if (ImGui::Selectable(viewLabels[i], _viewMode == i))
              _viewMode = (ViewMode)i;
          }
          ImGui::EndCombo();
        }
      }
      ImGui::Separator();
      ImGui::MenuItem("Assets Window", nullptr, &_showAssets);
      ImGui::MenuItem("Editor View Settings", nullptr, &_showEditorView);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
      if (ImGui::BeginMenu("Options"))
      {
        showOptions();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void
P3::gui()
{
  mainMenu();
  hierarchyWindow();
  inspectorWindow();
  assetsWindow();
  editorView();
}

inline void
drawMesh(GLMesh* mesh, GLuint mode)
{
  glPolygonMode(GL_FRONT_AND_BACK, mode);
  glDrawElements(GL_TRIANGLES, mesh->vertexCount(), GL_UNSIGNED_INT, 0);
}

inline void
P3::drawPrimitive(Primitive& primitive)
{
  auto m = glMesh(primitive.mesh());

  if (nullptr == m)
    return;

  auto t = primitive.transform();
  auto normalMatrix = mat3f{t->worldToLocalMatrix()}.transposed();

  _program[0].setUniformMat4("transform", t->localToWorldMatrix());
  _program[0].setUniformMat3("normalMatrix", normalMatrix);
  _program[0].setUniformVec4("color", primitive.material.diffuse);
  _program[0].setUniform("flatMode", (int)0);
  m->bind();
  drawMesh(m, GL_FILL);
  if (primitive.sceneObject() != _current)
    return;
  _program[0].setUniformVec4("color", _selectedWireframeColor);
  _program[0].setUniform("flatMode", (int)1);
  drawMesh(m, GL_LINE);
}

inline void
P3::drawLight(Light& light)
{
  // TODO
}

inline void
P3::drawCamera(Camera& camera)
{
  // TODO
}

inline void
P3::renderScene()
{
  if (auto camera = Camera::current())
  {
    _renderer->setCamera(camera);
    _renderer->setImageSize(width(), height());
    _renderer->render();
    _program[0].use();
  }
}


inline void
renderMesh(GLMesh* mesh, GLuint mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	glDrawElements(GL_TRIANGLES, mesh->vertexCount(), GL_UNSIGNED_INT, 0);
}


constexpr auto CAMERA_RES = 0.01f;
constexpr auto ZOOM_SCALE = 1.01f;

void
P3::render()
{
	if (_viewMode == ViewMode::Renderer)
	{
		renderScene();
		return;
	}
	if (_moveFlags)
	{
		const auto delta = _editor->orbitDistance() * CAMERA_RES;
		auto d = vec3f::null();

		if (_moveFlags.isSet(MoveBits::Forward))
			d.z -= delta;
		if (_moveFlags.isSet(MoveBits::Back))
			d.z += delta;
		if (_moveFlags.isSet(MoveBits::Left))
			d.x -= delta;
		if (_moveFlags.isSet(MoveBits::Right))
			d.x += delta;
		if (_moveFlags.isSet(MoveBits::Up))
			d.y += delta;
		if (_moveFlags.isSet(MoveBits::Down))
			d.y -= delta;
		_editor->pan(d);
	}
	_editor->newFrame();

	// **Begin rendering of temporary scene objects
	// It should be replaced by your rendering code (and moved to scene editor?)
	/*auto ec = _editor->camera();
	const auto& p = ec->transform()->position();
	auto vp = vpMatrix(ec);

	_program[0].setUniformMat4("vpMatrix", vp);
	_program[0].setUniformVec4("ambientLight", _scene->ambientLight);
	_program[0].setUniformVec3("lightPosition", p);
	for (const auto& o : _objects)
	{
		if (!o->visible)
			continue;

		auto component = o->component();

		if (auto p = dynamic_cast<Primitive*>(component))
			drawPrimitive(*p);
		else if (auto c = dynamic_cast<Camera*>(component))
			drawCamera(*c);
		if (o == _current)
		{
			auto t = o->transform();
			_editor->drawAxes(t->position(), mat3f{ t->rotation() });
		}
	}*/
	std::stack <std::pair<Reference<SceneObject>, int>> q;
	std::pair<Reference<SceneObject>, int> x;
	int quantidade_luz = 0;

	x = std::make_pair(_root, 0);
	Reference<SceneObject> f = x.first;
	for (int i = 0; i < f->childCount(); ++i)
		q.push(std::make_pair(f->getChild(i), x.second + 1));
	while (!q.empty())
	{
		x = q.top();
		f = x.first;
		q.pop();
		if (!f->visible)
		{
			for (int i = 0; i < f->childCount(); ++i)
				q.push(std::make_pair(f->getChild(i), x.second + 1));
			continue;
		}

		if (f->light() != nullptr && _b != Tonalizacao::None)
		{
			if (f->light()->type() == Light::Type::Directional && quantidade_luz < 11)
			{
				_program[_b].setUniformVec4((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].cor")).c_str(), f->light()->cor());
				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].direcao")).c_str(), f->light()->transform()->rotation() * vec3f { 0, -1.0f, 0 });

				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].posicao")).c_str(), vec3f(1, 1, 1));
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].fator")).c_str(), (int)0);
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_interno")).c_str(), (float)0);
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_externo")).c_str(), (float)0);
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].tipo")).c_str(), (int)Light::Type::Directional);
			}
			else if (f->light()->type() == Light::Type::Point && quantidade_luz < 11)
			{
				_program[_b].setUniformVec4((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].cor")).c_str(), f->light()->cor());
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].fator")).c_str(), (int)f->light()->fator());
				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].posicao")).c_str(), f->transform()->position());

				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].direcao")).c_str(), vec3f(1, 1, 1));
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_interno")).c_str(), (float)0);
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_externo")).c_str(), (float)0);
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].tipo")).c_str(), (int)Light::Type::Point);
			}
			else if (f->light()->type() == Light::Type::Spot && quantidade_luz < 11)
			{
				_program[_b].setUniformVec4((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].cor")).c_str(), f->light()->cor());
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].fator")).c_str(), (int)f->light()->fator());
				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].posicao")).c_str(), f->transform()->position());
				_program[_b].setUniformVec3((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].direcao")).c_str(), f->light()->transform()->rotation() * vec3f { 0, -1.0f, 0 });
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_interno")).c_str(), (float)cos(M_PI * f->light()->angulo_interno() / 180));
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].angulo_externo")).c_str(), (float)cos(M_PI * f->light()->angulo_externo() / 180));
				_program[_b].setUniform((std::string("luz[") + std::to_string(quantidade_luz) + std::string("].tipo")).c_str(), (int)Light::Type::Spot);
			}
			quantidade_luz++;
		}

		if (f->primitive() == nullptr)
		{
			for (int i = 0; i < f->childCount(); ++i)
				q.push(std::make_pair(f->getChild(i), x.second + 1));
			continue;
		}
		auto m = glMesh(f->primitive()->mesh());

		for (int i = 0; i < f->childCount(); ++i)
			q.push(std::make_pair(f->getChild(i), x.second + 1));

		if (nullptr == m)
			continue;

		auto t = f->transform();
		auto normalMatrix = mat3f{ t->worldToLocalMatrix() }.transposed();

		_program[_b].setUniformMat4("transform", t->localToWorldMatrix());

		if (_currentObj == f)
		{
			m->bind();
			if (_b != Tonalizacao::None)
			{
				//_program[_b].setUniformVec4("material.diffuse", _selectedWireframeColor);
				_program[_b].setUniform("flatMode", (int)1);
			}
			else
				_program[_b].setUniformVec4("color", _selectedWireframeColor);
			renderMesh(m, GL_LINE);
		}

		if (_b == Tonalizacao::Gouraud || _b == Tonalizacao::Phong)
			_program[_b].setUniformMat3("normalMatrix", normalMatrix);
		if (_b != Tonalizacao::None)
		{
			/*_program[_b].setUniformVec4("material.ambient", f->primitive()->material.ambient);
			_program[_b].setUniformVec4("material.diffuse", f->primitive()->material.diffuse);
			_program[_b].setUniformVec4("material.spot", f->primitive()->material.spot);
			_program[_b].setUniform("material.shine", (float)f->primitive()->material.shine);*/
			_program[_b].setUniform("flatMode", (int)0);
		}
		else
			_program[_b].setUniformVec4("color", _corArestas);
		m->bind();
		if (_b != Tonalizacao::None)
			renderMesh(m, GL_FILL);
		else
			renderMesh(m, GL_LINE);


	}
	if (_b != Tonalizacao::None)
	{
		_program[_b].setUniform("quantidade_luz", (int)quantidade_luz);
		_program[_b].setUniformVec3("view_position", _camera->position());
		_program[_b].setUniformVec4("ambientLight", _scene->ambientLight);
	}
	_program[_b].setUniformMat4("vpMatrix", vpMatrix(_camera));

	// **End rendering of temporary scene objects
}
bool
P3::windowResizeEvent(int width, int height)
{
  _editor->camera()->setAspectRatio(float(width) / float(height));
  return true;
}

bool
P3::keyInputEvent(int key, int action, int mods)
{
  auto active = action != GLFW_RELEASE && mods == GLFW_MOD_ALT;

  switch (key)
  {
    case GLFW_KEY_W:
      _moveFlags.enable(MoveBits::Forward, active);
      break;
    case GLFW_KEY_S:
      _moveFlags.enable(MoveBits::Back, active);
      break;
    case GLFW_KEY_A:
      _moveFlags.enable(MoveBits::Left, active);
      break;
    case GLFW_KEY_D:
      _moveFlags.enable(MoveBits::Right, active);
      break;
    case GLFW_KEY_Q:
      _moveFlags.enable(MoveBits::Up, active);
      break;
    case GLFW_KEY_Z:
      _moveFlags.enable(MoveBits::Down, active);
      break;
  }
  return false;
}

bool
P3::scrollEvent(double, double yOffset)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  _editor->zoom(yOffset < 0 ? 1.0f / ZOOM_SCALE : ZOOM_SCALE);
  return true;
}

bool
P3::mouseButtonInputEvent(int button, int actions, int mods)
{
  if (ImGui::GetIO().WantCaptureMouse)
    return false;
  (void)mods;

  auto active = actions == GLFW_PRESS;

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
    _dragFlags.enable(DragBits::Rotate, active);
  else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    _dragFlags.enable(DragBits::Pan, active);
  if (_dragFlags)
    cursorPosition(_pivotX, _pivotY);
  return true;
}

bool
P3::mouseMoveEvent(double xPos, double yPos)
{
  if (!_dragFlags)
    return false;
  _mouseX = (int)xPos;
  _mouseY = (int)yPos;

  const auto dx = (_pivotX - _mouseX);
  const auto dy = (_pivotY - _mouseY);

  _pivotX = _mouseX;
  _pivotY = _mouseY;
  if (dx != 0 || dy != 0)
  {
    if (_dragFlags.isSet(DragBits::Rotate))
    {
      const auto da = -_editor->camera()->viewAngle() * CAMERA_RES;
      isKeyPressed(GLFW_KEY_LEFT_ALT) ?
        _editor->orbit(dy * da, dx * da) :
        _editor->rotateView(dy * da, dx * da);
    }
    if (_dragFlags.isSet(DragBits::Pan))
    {
      const auto dt = -_editor->orbitDistance() * CAMERA_RES;
      _editor->pan(-dt * math::sign(dx), dt * math::sign(dy), 0);
    }
  }
  return true;
}

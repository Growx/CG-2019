#include "P1.h"

inline void
P1::buildScene()
{
  _current = _scene = new Scene{"Scene 1"};
  _box = new SceneObject{"Box 1", _scene};
  _primitive = makeBoxMesh();

}

void
P1::initialize()
{
  Application::loadShaders(_program, "p1.vs", "p1.fs");
  buildScene();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  glLineWidth(2.0f);
  glEnable(GL_LINE_SMOOTH);
  _program.use();
}

namespace ImGui
{
  void ShowDemoWindow(bool*);
}

inline void
P1::hierarchyWindow()
{
  ImGui::Begin("Hierarchy");
  if (ImGui::Button("Create###object"))
    ImGui::OpenPopup("CreateObjectPopup");
  if (ImGui::BeginPopup("CreateObjectPopup"))
  {
    ImGui::MenuItem("Empty Object");
    if (ImGui::BeginMenu("3D Object"))
    {
      if (ImGui::MenuItem("Box"))
      {
        // TODO: create a new box.
		  //ObjectNode(_scene);
		  makeBoxMesh();
      }
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
  ImGui::Separator();

  ImGuiTreeNodeFlags flag{ImGuiTreeNodeFlags_OpenOnArrow};
  auto open = ImGui::TreeNodeEx(_scene,
    _current == _scene ? flag | ImGuiTreeNodeFlags_Selected : flag,
    _scene->name());

  if (ImGui::IsItemClicked())
    _current = _scene;
  if (open)
  {
    flag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(_box,
      _current == _box ? flag | ImGuiTreeNodeFlags_Selected : flag,
      _box->name());
    if (ImGui::IsItemClicked())
      _current = _box;
    ImGui::TreePop();
  }
  ImGui::End();
}

namespace ImGui
{ // begin namespace ImGui

void
ObjectNameInput(SceneNode* object)
{
  const int bufferSize{128};
  static SceneNode* current;
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
P1::sceneGui()
{
  auto scene = (Scene*)_current;

  ImGui::ObjectNameInput(_current);
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorEdit3("Background", backgroundColor);
    ImGui::ColorEdit3("Selected Wireframe", selectedWireframeColor);
  }
}

inline void
P1::inspectPrimitive(Primitive& primitive)
{
	ImGui::ColorEdit3("Mesh Color", (float*)& primitive.color);
}
inline void
P1::sceneObjectGui()
{
  auto object = (SceneObject*)_current;

  ImGui::ObjectNameInput(object);
  ImGui::SameLine();
  ImGui::Checkbox("###visible", &object->visible);
  ImGui::Separator();
  if (ImGui::CollapsingHeader(object->transform()->typeName()))
  {
    auto t = object->transform();

    ImGui::TransformEdit(t);
    _transform = t->localToWorldMatrix();
  }
  if (ImGui::CollapsingHeader(_primitive->typeName()))
  {
    // TODO: show primitive properties.
	  inspectPrimitive(*_primitive);
  }
}

inline void
P1::objectGui()
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
P1::inspectorWindow()
{
  ImGui::Begin("Inspector");
  objectGui();
  ImGui::End();
}

void
P1::gui()
{
  hierarchyWindow();
  inspectorWindow();
  
 /* static bool demo = true;
  ImGui::ShowDemoWindow(&demo);
  */
}

void
P1::render()
{
  GLWindow::render();
  if (!_box->visible)
    return;
  _program.setUniformMat4("transform", _transform);
  auto m = _primitive->mesh();

  m->bind();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
  if (_current != _box)
    return;
 
  m->setVertexColor(selectedWireframeColor);  
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
  m->setVertexColor(_primitive->color);

}

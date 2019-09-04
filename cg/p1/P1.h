#ifndef __P1_h
#define __P1_h

#include "Primitive.h"
#include "Scene.h"
#include "graphics/Application.h"
#include "DLinkedList.h"

using namespace cg;

class P1: public GLWindow
{
public:
  P1(int width, int height):
    GLWindow{"cg2019 - P1", width, height},
    _program{"P1"}
  {
    // do nothing
  }

  /// Initialize the app.
  void initialize() override;

  /// Update the GUI.
  void gui() override;

  /// Render the scene.
  void render() override;

  DLLNode<SceneObject> ObjectNode(Scene* scene) {
	  _current = scene;
	  _box = new SceneObject("Box", scene);
	  _primitive = makeBoxMesh();
	  //ObjectList.addToDLLHead(*_box);
}
  DLinkedList<SceneObject> ObjectList;

private:
  GLSL::Program _program;
  Reference<Scene> _scene;
  Reference<SceneObject> _box;
  Reference<Primitive> _primitive;
  SceneNode* _current{};
  Color selectedWireframeColor{255, 102, 0};
  mat4f _transform{mat4f::identity()};


  void buildScene();

  void hierarchyWindow();
  void inspectorWindow();
  void inspectPrimitive(Primitive&);
  void sceneGui();
  void sceneObjectGui();
  void objectGui();

  inline Primitive*
	  makeBoxMesh()
  {
	  const vec4f p1{ -0.5, -0.5, -0.5, 1 };
	  const vec4f p2{ +0.5, -0.5, -0.5, 1 };
	  const vec4f p3{ +0.5, +0.5, -0.5, 1 };
	  const vec4f p4{ -0.5, +0.5, -0.5, 1 };
	  const vec4f p5{ -0.5, -0.5, +0.5, 1 };
	  const vec4f p6{ +0.5, -0.5, +0.5, 1 };
	  const vec4f p7{ +0.5, +0.5, +0.5, 1 };
	  const vec4f p8{ -0.5, +0.5, +0.5, 1 };
	  const Color c1{ Color::black };
	  const Color c2{ Color::red };
	  const Color c3{ Color::yellow };
	  const Color c4{ Color::green };
	  const Color c5{ Color::blue };
	  const Color c6{ Color::magenta };
	  const Color c7{ Color::cyan };
	  const Color c8{ Color::white };

	  // Box vertices
	  static const vec4f v[]
	  {
		p1, p5, p8, p4, // x = -0.5
		p2, p3, p7, p6, // x = +0.5
		p1, p2, p6, p5, // y = -0.5
		p4, p8, p7, p3, // y = +0.5
		p1, p4, p3, p2, // z = -0.5
		p5, p6, p7, p8  // z = +0.5
	  };

	  // Box vertex colors
	  static const Color c[]
	  {
		c1, c5, c8, c4, // x = -0.5
		c2, c3, c7, c6, // x = +0.5
		c1, c2, c6, c5, // y = -0.5
		c4, c8, c7, c3, // y = +0.5
		c1, c4, c3, c2, // z = -0.5
		c5, c6, c7, c8  // z = +0.5
	  };

	  // Box triangles
	  static const GLMeshArray::Triangle t[]
	  {
		{ 0,  1,  2}, { 2,  3,  0},
		{ 4,  5,  7}, { 5,  6,  7},
		{ 8,  9, 11}, { 9, 10, 11},
		{12, 13, 14}, {14, 15, 12},
		{16, 17, 19}, {17, 18, 19},
		{20, 21, 22}, {22, 23, 20}
	  };

	  return new Primitive{ new GLMeshArray{24, {v, 0}, {c, 1}, 12, t} };
  }
}; // P1

#endif // __P1_h

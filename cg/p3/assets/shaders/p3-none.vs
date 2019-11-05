#version 330 core
uniform mat4 transform = mat4(1);
uniform mat4 vpMatrix;
uniform vec4 color;
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
out vec4 vertexColor;
void main()
{
  gl_Position = vpMatrix * transform * position;
  vertexColor = color;
}

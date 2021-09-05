#version 450

layout(binding = 0) uniform UniformScene {
  mat4 projection;
  mat4 view;
} scene;

layout(binding = 1) uniform UniformObject {
  mat4 transform;

  vec4 color;
} object;

layout(location = 0) out vec4 outColor;

void main() {
  outColor = object.color; 
}

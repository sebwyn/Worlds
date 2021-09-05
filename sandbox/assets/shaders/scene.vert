#version 450

layout(binding = 0) uniform UniformScene {
  mat4 projection;
  mat4 view;
} scene;

layout(binding = 1) uniform UniformObject {
  mat4 transform;

  vec4 color;
} object;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

void main() {
  gl_Position = scene.projection * scene.view * object.transform * vec4(position, 1);
}

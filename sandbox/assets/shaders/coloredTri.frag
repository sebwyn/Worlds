#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformObject { 
    vec4 color;
} object;

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(inColor);
    //outColor = vec4(1, 1, 1, 1);
}

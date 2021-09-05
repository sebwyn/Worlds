#version 450

layout(binding = 0) uniform UniformObject {
  vec4 color1;
  vec4 color2;
  vec4 color3;
} object;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

layout(location = 0) out vec4 outColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
  
    if(gl_VertexIndex == 0){
        outColor = object.color1;
    }else if(gl_VertexIndex == 1){
        outColor = vec4(1, 1, 1, 1);
    }else if(gl_VertexIndex == 2){
        outColor = vec4(0, 0, 0, 1);
    }
}

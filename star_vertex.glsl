#version 450 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    // Remove translation from view matrix for skybox effect
    mat4 rotView = mat4(mat3(uView));
    vec4 pos = uProjection * rotView * vec4(aPosition, 1.0);
    
    // Ensure stars are always rendered at maximum depth
    gl_Position = pos.xyww;
    gl_PointSize = 2.0;
}

#version 450 core

out vec4 FragColor;

void main() {
    // Create a circular star point
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);
    
    if (dist > 0.5) {
        discard;
    }
    
    float alpha = 1.0 - smoothstep(0.0, 0.5, dist);
    FragColor = vec4(1.0, 1.0, 0.9, alpha);
}

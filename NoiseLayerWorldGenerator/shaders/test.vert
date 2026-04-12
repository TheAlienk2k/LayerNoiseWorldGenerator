#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;      // Gdzie jest obiekt
uniform mat4 view;       // Gdzie jest kamera
uniform mat4 projection; // Perspektywa

void main() {
    // Kolejność mnożenia macierzy ma znaczenie! (od prawej do lewej)
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
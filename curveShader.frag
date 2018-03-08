#version 330 core
// This is a sample fragment shader.
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
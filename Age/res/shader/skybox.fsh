#version 330 core

out vec4 FragColor;

in vec3 fragUv;
uniform sampler3D skybox;

void main()
{
    FragColor = vec4(texture(skybox, fragUv), 1.f);
}

out vec4 FragColor;

uniform vec3 sceneAmbient, emission;

void main()
{
    FragColor = vec4(emission, 1.f);
}

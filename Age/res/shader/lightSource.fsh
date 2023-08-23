out vec4 FragColor;

uniform vec3 sceneAmbient, lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.f);
}

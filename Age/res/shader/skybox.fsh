out vec4 FragColor;

in vec3 fragUv;
uniform samplerCube skybox;

void main()
{
    FragColor = vec4(texture(skybox, normalize(fragUv)).rgb, 1.f);
}

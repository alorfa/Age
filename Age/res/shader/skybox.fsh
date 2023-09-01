out vec4 FragColor;

in vec3 fragUv;
uniform samplerCube skybox;

vec4 sampleCubemap(samplerCube cubemap, vec3 texCoord)
{
	vec3 coord = vec3(texCoord.x, -texCoord.z, texCoord.y);
	return texture(cubemap, coord);
}

void main()
{
    FragColor = sampleCubemap(skybox, fragUv);
}

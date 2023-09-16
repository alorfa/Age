#define AGE_FRAGMENT

#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;

uniform mat4 view, projection;

out vec3 fragUv;

void main()
{
	fragUv = inPosition;
	vec4 pos = projection * view * vec4(inPosition, 1.f);
	gl_Position = pos.xyww;
}

#endif

#ifdef AGE_FRAGMENT

#define AGE_CUSTOM_PAINTING_OVER

out vec4 age_FragColor;

in vec3 fragUv;
uniform samplerCube skybox;

vec4 sampleCubemap(samplerCube cubemap, vec3 texCoord)
{
	vec3 coord = vec3(texCoord.x, -texCoord.z, texCoord.y);
	return texture(cubemap, coord);
}

void control()
{

}

void paintOver()
{
    age_FragColor = sampleCubemap(skybox, fragUv);
}

void main()
{
	control();
	paintOver();
}

#endif

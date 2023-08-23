layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;

uniform mat4 model, view, projection;
//uniform mat4 viewProjection;

out vec2 fragUv;
out vec3 fragPos, fragNormal;

void main()
{
	fragUv = inUv;
	fragPos = (model * vec4(inPosition, 1.f)).xyz;
	//fragNormal = mat3(transpose(inverse(model))) * normal;
	//viewProjection = projection * view;
	//vec3 pos = position + normalize(normal) * 0.05f;
	fragNormal = mat3(model) * inNormal;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}

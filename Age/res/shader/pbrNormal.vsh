#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
layout (location = 3) in vec3 inTangent;

uniform mat4 model, view, projection;

out vec2 fragUv;
out vec3 fragPos;
out mat3 TBN;

void main()
{
	fragUv = inUv;
	fragPos = (model * vec4(inPosition, 1.f)).xyz;
	vec3 N = mat3(model) * inNormal;
	vec3 T = mat3(model) * inTangent;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}

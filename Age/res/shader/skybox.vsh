layout (location = 0) in vec3 inPosition;

uniform mat4 view, projection;

out vec3 fragUv;

void main()
{
	fragUv = inPosition;
	vec4 pos = projection * view * vec4(inPosition, 1.f);
	gl_Position = pos.xyww;
}

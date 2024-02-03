layout (location = 0) in vec2 position;

out vec2 uv;

void main()
{
	uv = position * 0.5 + 0.5;
    gl_Position = vec4(position.x, position.y, 0.f, 1.f);
}

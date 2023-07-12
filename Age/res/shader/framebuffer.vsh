#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 uv;

void main()
{
	uv = texCoord;
    gl_Position = vec4(position.x, position.y, 0.f, 1.f);
}

#version 330 core

out vec4 FragColor;

in vec2 uv;
uniform sampler2D tex;

const float gamma = 2.2f;

void main()
{
    vec3 outColor = texture(tex, uv).rgb;
    //outColor.r = texture(tex, uv + vec2(0.003f, 0.003f)).r;

    //FragColor = vec4(outColor, 1.f);
    FragColor = vec4(pow(outColor, vec3(1.f / gamma)), 1.0f);
}

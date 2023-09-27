#define AGE_VERTEX
#define AGE_MAX_DIR_LIGHTS 0
#define AGE_MAX_POINT_LIGHTS 0
#define AGE_MAX_SPOT_LIGHTS 1
#define AGE_RENDERING_MODE_FORWARD


uniform mat4 model, view, projection;

out vec2 fragUv;
out vec3 fragPos;
out mat3 fragTBN;

void computeTBN(in vec3 normal, in vec3 tangent)
{
	vec3 N = mat3(model) * normal;
    vec3 T = mat3(model) * tangent;
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    fragTBN = mat3(T, B, N);
}
#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
layout (location = 3) in vec3 inTangent;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    computeTBN(inNormal, inTangent);
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform sampler2D textures[8];

void fragmentControl()
{
    age_base_color = texture(textures[0], fragUv).rgb;
    age_specular = vec3(0.07f);
    age_normal = computeNormal(texture(textures[1], fragUv).rgb);
    //age_normal = normalize(fragTBN[2]);
    vec4 material = texture(textures[2], fragUv);
    age_roughness = material.g;
    age_shininess = 4.f;
    age_metalness = material.b;
}
#define AGE_LIGHT_MODE_PHONG

#endif
void main()
{
	gl_Position = vertexProcess();
}
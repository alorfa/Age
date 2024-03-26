#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inUv;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.)).xyz;
    return projectionView * vec4(fragPos, 1.);
}

#endif
#ifdef AGE_FRAGMENT

uniform sampler2D baseColorMap, normalMap;

void setup()
{
	vec4 albedoRoughness = texture(baseColorMap, fragUv);
	vec4 normalAo = texture(normalMap, fragUv);
    material.base_color = albedoRoughness.rgb;
    material.roughness = albedoRoughness.a;
    material.normal = normalize(normalAo.rgb * 2 - 1);
    material.ao = normalAo.a;
}

#endif
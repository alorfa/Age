void control()
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

#version 450 
#extension GL_EXT_nonuniform_qualifier : enable

#define NUM_LIGHTS 8

struct Light {
    vec4 position;
    vec3 color;
    float radius;
};

layout(binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
    vec3 viewPosition;
    vec3 ambient;
    int numLights;
    Light lights[NUM_LIGHTS];
} ubo;

layout(binding = 1) uniform sampler2D texSampler[];

layout (push_constant) uniform constants {
    mat4 model;
    int texId;
    float specular;
    float shininess;
} pushConstants;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord0;
layout(location = 3) in vec2 fragTexCoord1;
layout(location = 4) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

void main(){
    vec4 albedo = texture(texSampler[pushConstants.texId], fragTexCoord0);
    vec3 finalColor = albedo.rgb * vec3(0.5, 0.5, 0.5);// * ubo.ambient;

    for (int i = 0; i < ubo.numLights; ++i) {
        vec3 L = ubo.lights[i].position.xyz - fragPosition; // Fragment to Light vector
        float sqrDistance = dot(L, L); // Fragment to Light square distance
        L = normalize (L); // Fragment to Light normal
        vec3 V = normalize(ubo.viewPosition- fragPosition); // Fragment to Viewer normal

        // Attenuation
        float attenuation = ubo.lights[i].radius / (sqrDistance + 1.0);

        // Diffuse
        finalColor += ubo.lights[i].color * albedo.rgb * max(dot(vertNormal, L), 0.0) * attenuation;

        // Specular
        vec3 R = reflect(-L, vertNormal); // Light reflection normal
        finalColor += ubo.lights[i].color * pushConstants.specular * pow(max(dot(R, V), 0.0), pushConstants.shininess) * attenuation;
    }

    outColor = vec4(finalColor, 1);
}
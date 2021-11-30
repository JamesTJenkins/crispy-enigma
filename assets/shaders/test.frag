#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord0;
layout(location = 2) in vec2 fragTexCoord1;

layout(location = 0) out vec4 outColor;

void main(){
    outColor = texture(texSampler, fragTexCoord0);
}
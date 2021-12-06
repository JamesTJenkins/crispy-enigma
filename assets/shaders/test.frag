#version 450 
#extension GL_EXT_nonuniform_qualifier : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
} ubo;

layout(binding = 1) uniform sampler2D texSampler[];

layout (push_constant) uniform constants {
    mat4 model;
    int texId;
} pushConstants;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord0;
layout(location = 3) in vec2 fragTexCoord1;

layout(location = 0) out vec4 outColor;

void main(){
    outColor = texture(texSampler[pushConstants.texId], fragTexCoord0);
}
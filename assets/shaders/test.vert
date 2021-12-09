#version 450

layout(binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
} ubo;

layout (push_constant) uniform constants {
    mat4 model;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord0;
layout(location = 4) in vec2 inTexCoord1;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord0;
layout(location = 3) out vec2 fragTexCoord1;
layout(location = 4) out vec3 fragPosition;


void main(){
    vec4 pos = ubo.proj * ubo.view * pushConstants.model * vec4(inPosition, 1.0);
	gl_Position = pos;
	
    vertNormal = inNormal;
	fragColor = inColor;
	fragTexCoord0 = inTexCoord0;
    fragTexCoord1 = inTexCoord1;
    fragPosition = (pushConstants.model * vec4(inPosition, 1.0)).xyz;
}
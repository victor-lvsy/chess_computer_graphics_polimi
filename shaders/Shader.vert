#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
} ubo;

layout(set = 0, binding = 2) uniform UniformBufferColor{
	float trigger;
	vec2 uvColor;
} color;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 fragNormal;

void main() {
	gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
	if(color.trigger == 1.0f){
		outUV = color.uvColor;
	}
	else{
		outUV = inUV;
	}
	fragPos = inPosition; // Assuming position in model space
    fragNormal = normalize(inNormal); // Using the provided normal
}
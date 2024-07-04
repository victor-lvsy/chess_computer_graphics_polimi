#version 450
#extension GL_ARB_separate_shader_objects : enable

	mat4 mvpMat;
} ubo;

layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;

	gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
	outUV = inUV;
}
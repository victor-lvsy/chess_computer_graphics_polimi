#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D tex;
layout(set = 1, binding = 0) uniform GlobalUniformBufferObject {
    vec3 lightDir[5];
    vec3 lightPos[5];
    vec4 lightColor;
    float cosIn;
    float cosOut;
    vec3 eyePos;
    vec4 eyeDir;
} gubo;


vec3 direct_light_dir(vec3 pos, int i) {
    // Direct light - direction vector
    return gubo.lightDir[i];
}

vec3 direct_light_color(vec3 pos, int i) {
    // Direct light - color
    return gubo.lightColor.rgb * 1.5; // Increase the light intensity
}

vec3 spot_light_dir(vec3 pos, int i) {
    // Spot light - direction vector
    return normalize(gubo.lightPos[i] - pos);
}

vec3 spot_light_color(vec3 pos, int i) {
    // Spot light - color
    float distance = length(gubo.lightPos[i] - pos);
    vec3 lightDirection = normalize(gubo.lightPos[i] - pos);
    float theta = dot(lightDirection, normalize(-gubo.lightDir[i]));
    float epsilon = gubo.cosIn - gubo.cosOut;
    float intensity = clamp((theta - gubo.cosOut) / epsilon, 0.0, 1.0);
    vec3 attenuation = gubo.lightColor.rgb * pow(gubo.lightColor.a / distance, 1.0); // Reduce the power for attenuation
    return attenuation * intensity;
}

vec3 BRDF(vec3 Albedo, vec3 Norm, vec3 EyeDir, vec3 LD) {
    // Compute the BRDF, with a given color <Albedo>, in a given position characterized by a given normal vector <Norm>,
    // for a light direct according to <LD>, and viewed from a direction <EyeDir>
    vec3 Diffuse = Albedo * max(dot(Norm, LD), 0.0);
    vec3 Specular = vec3(pow(max(dot(EyeDir, reflect(-LD, Norm)), 0.0), 32.0));
    return Diffuse + Specular;
}

void main() {
    vec3 Norm = normalize(fragNormal);
    vec3 EyeDir = normalize(gubo.eyePos - fragPos);
    vec3 Albedo = texture(tex, fragUV).rgb;

    vec3 LD;    // light direction
    vec3 LC;    // light color

    vec3 RendEqSol = vec3(0.0);

    // Calculate direct light contribution
    LD = direct_light_dir(fragPos, 0);
    LC = direct_light_color(fragPos, 0);
    RendEqSol += BRDF(Albedo, Norm, EyeDir, LD) * LC;

    // Calculate spotlight contribution

    LD = spot_light_dir(fragPos, 1);
    LC = spot_light_color(fragPos, 1);
    RendEqSol += BRDF(Albedo, Norm, EyeDir, LD) * LC;

    LD = spot_light_dir(fragPos, 2);
    LC = spot_light_color(fragPos, 2);
    RendEqSol += BRDF(Albedo, Norm, EyeDir, LD) * LC;

    LD = spot_light_dir(fragPos, 3);
    LC = spot_light_color(fragPos, 3);
    RendEqSol += BRDF(Albedo, Norm, EyeDir, LD) * LC;

    LD = spot_light_dir(fragPos, 4);
    LC = spot_light_color(fragPos, 4);
    RendEqSol += BRDF(Albedo, Norm, EyeDir, LD) * LC;

    // Indirect illumination simulation

    const vec3 cxp = vec3(1.0, 0.8, 0.8) * 0.8;
    const vec3 cxn = vec3(0.9, 0.7, 0.5) * 0.8;
    const vec3 cyp = vec3(0.5, 1.0, 1.0) * 0.8; 
    const vec3 cyn = vec3(0.7, 0.7, 0.7) * 0.8;
    const vec3 czp = vec3(0.9, 0.4, 0.5) * 0.8;
    const vec3 czn = vec3(0.5, 0.7, 0.8) * 0.8;

    vec3 Ambient = ((Norm.x > 0 ? cxp : cxn) * (Norm.x * Norm.x) +
                    (Norm.y > 0 ? cyp : cyn) * (Norm.y * Norm.y) +
                    (Norm.z > 0 ? czp : czn) * (Norm.z * Norm.z)) * Albedo;
    RendEqSol += Ambient;

    // Output color
    outColor = vec4(RendEqSol, 1.0f);
}
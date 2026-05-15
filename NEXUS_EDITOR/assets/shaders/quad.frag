#version 440 core

// Inputs
layout (location = 0) in vec2 inUVs;
layout (location = 1) in vec4 inColor;

// Outputs
layout (location = 0) out vec4 outColor;

// Uniforms
uniform sampler2D uTextureSampler;

void main()
{
  vec4 texColor = texture(uTextureSampler, inUVs);
  outColor = texColor * inColor;
}

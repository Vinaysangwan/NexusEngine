#version 440 core

// Inputs
layout (location = 0) in vec2 inTexCoords;

// Outputs
layout (location = 0) out vec4 outColor;

// Uniforms
uniform sampler2D uTextureSampler;

void main()
{
  vec4 texColor = texture(uTextureSampler, inTexCoords);
  outColor = texColor;
}

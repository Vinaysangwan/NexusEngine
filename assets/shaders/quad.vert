#version 440 core

// Inputs
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUVs;
layout (location = 2) in vec4 inColor;

// Outputs
layout (location = 0) out vec2 outUVs;
layout (location = 1) out vec4 outColor;

// Uniforms
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * vec4(inPos, 1.0f);

  outUVs = inUVs;
  outColor = inColor;
}

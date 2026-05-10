#version 440 core

// Inputs
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;

// Outputs
layout (location = 0) out vec2 outTexCoords;

// Uniforms
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * vec4(inPos, 1.0f);

  outTexCoords = inTexCoords;
}

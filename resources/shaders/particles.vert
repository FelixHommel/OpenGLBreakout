#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 uProjection;
uniform vec2 uOffset;
uniform vec4 uColor;

out vec2 texCoords;
out vec4 particleColor;

void main()
{
    float scale = 10.f;
    texCoords = vertex.zw;
    particleColor = uColor;

    gl_Position = uProjection * vec4((vertex.xy * scale) + uOffset, 0.f, 1.f);
}

#version 330 core

in vec2 texCoords;
in vec4 particleColor;

uniform sampler2D uTexture;

out vec4 color;

void main()
{
    color = (texture(uTexture, texCoords) * particleColor);
}

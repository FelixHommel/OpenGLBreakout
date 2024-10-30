#version 330 core

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec3 uColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(uColor, 1.f) * texture(uTexture, TexCoord);
}


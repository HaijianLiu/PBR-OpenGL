#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureR;
uniform sampler2D textureG;
uniform sampler2D textureB;

void main()
{
    float r = texture(textureR, TexCoords).r;
		float g = texture(textureG, TexCoords).g;
		float b = texture(textureB, TexCoords).b;

    FragColor = vec4(r, g, b, 1.0);
}

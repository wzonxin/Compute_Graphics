#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	fragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.1);
}
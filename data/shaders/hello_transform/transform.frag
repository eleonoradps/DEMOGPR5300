#version 330 core
layout(location = 0) out vec4 FragColor;
in vec3 outColor;
in vec2 outTexture;
uniform sampler2D ourTexture;
uniform float colorCoeff;

void main()
{
	FragColor = texture(ourTexture, outTexture) * vec4(outColor * colorCoeff, 1.0f);
}
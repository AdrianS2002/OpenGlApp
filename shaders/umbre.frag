#version 410 core
in vec2 fTexCoords;
out vec4 fColor;
uniform sampler2D diffuseTexture;
void main()
{
	vec4 colorFromTexture= texture(diffuseTexture, fTexCoords);

	if(colorFromTexture.a < 0.1f)
		discard;
	fColor = vec4(1.0f);
}


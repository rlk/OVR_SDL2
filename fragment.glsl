#version 150

uniform vec4 LightPosition;
uniform vec3 AmbientLight;

in vec4 fPosition;
in vec3 fNormal;
in vec3 fColor;

out vec4 fOutput;

void main()
{
	vec3 L = normalize(LightPosition.xyz - fPosition.xyz);
	vec3 N = normalize(fNormal);

	float k = max(dot(L, N), 0.0);

    fOutput = vec4(fColor * k + AmbientLight, 1.0);
}

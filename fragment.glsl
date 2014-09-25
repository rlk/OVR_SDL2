#version 150

uniform vec4 LightPosition;
uniform vec3 AmbientLight;

in vec4 fPosition;
in vec3 fNormal;
in vec3 fColor;
in vec3 fChecker;
in vec3 fCoord;

out vec4 fOutput;

void main()
{
    vec3 L = normalize(LightPosition.xyz - fPosition.xyz);
    vec3 N = normalize(fNormal);

    vec3 c = step(fChecker.y, fract(fCoord * fChecker.x));
    float k = c.x + c.z - 2.0 * c.x * c.z;

    float d = max(dot(L, N), 0.0);

    fOutput = vec4(mix(fColor, fColor * fChecker.z, k) * (d + AmbientLight), 1.0);
}

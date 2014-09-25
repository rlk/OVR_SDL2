#version 150

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

in  vec4 vPosition;
in  vec3 vNormal;
in  vec3 vColor;
in  vec3 vChecker;

out vec4 fPosition;
out vec3 fNormal;
out vec3 fColor;
out vec3 fChecker;
out vec3 fCoord;

void main()
{
	fPosition = ModelViewMatrix * vPosition;
    fNormal   = NormalMatrix    * vNormal;
    fColor    =                   vColor;
    fChecker  =                   vChecker;
    fCoord    =                   vPosition.xyz;

    gl_Position = ProjectionMatrix * ModelViewMatrix * vPosition;
}

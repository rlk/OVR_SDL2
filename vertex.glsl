#version 150

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

in  vec4 vPosition;
in  vec3 vNormal;
in  vec3 vColor;

out vec4 fPosition;
out vec3 fNormal;
out vec3 fColor;

void main()
{
	fPosition = ModelViewMatrix * vPosition;
    fNormal   = NormalMatrix    * vNormal;
    fColor    =                   vColor;

    gl_Position = ProjectionMatrix * ModelViewMatrix * vPosition;
}

// Copyright (c) 2014 Robert Kooima
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "OVR_SDL2_obj.hpp"

//------------------------------------------------------------------------------

const char *vert_src = R"(
    #version 150

    uniform mat4 ProjectionMatrix;
    uniform mat4 ModelViewMatrix;
    uniform mat3 NormalMatrix;
    uniform vec4 LightPosition;

    in vec4 vPosition;
    in vec3 vNormal;
    in vec3 vTangent;
    in vec2 vTexCoord;

    out vec3 fView;
    out vec3 fLight;
    out vec2 fTexCoord;

    void main()
    {
        // Tangent space vectors give columns of the eye-to-tangent matrix.

        vec3 N = NormalMatrix * vNormal;
        vec3 T = NormalMatrix * vTangent;
        mat3 M = transpose(mat3(T, cross(N, T), N));

        // Compute the per-fragment attributes.

        fView     =  M * vec3(ModelViewMatrix * vPosition);
        fLight    =  M * vec3(ModelViewMatrix * LightPosition);
        fTexCoord =  vTexCoord;

        gl_Position = ProjectionMatrix * ModelViewMatrix * vPosition;
    }
)";

const char *frag_src = R"(
    #version 150

    uniform vec4 AmbientLight;

    uniform sampler2D NormalTexture;
    uniform sampler2D DiffuseTexture;
    uniform sampler2D SpecularTexture;

    in vec3 fView;
    in vec3 fLight;
    in vec2 fTexCoord;

    out vec4 fOutput;

    void main()
    {
        // Sample the textures.

        vec4 tN = texture(NormalTexture,   fTexCoord);
        vec4 tD = texture(DiffuseTexture,  fTexCoord);
        vec4 tS = texture(SpecularTexture, fTexCoord);

        // Determine the per-fragment lighting vectors.

        vec3 N = normalize(2.0 * tN.xyz - 1.0);
        vec3 L = normalize(fLight);
        vec3 V = normalize(fView);
        vec3 R = reflect(L, N);

        // Compute the diffuse shading.

        float kd =     max(dot(L, N), 0.0);
        float ks = pow(max(dot(V, R), 0.0), 8.0);

        // Calculate the fragment color.

        fOutput.rgb = vec3(AmbientLight * tD + kd * tD + tS * ks);
        fOutput.a   = tD.a;
    }
)";

//------------------------------------------------------------------------------

OVR_SDL2_obj::OVR_SDL2_obj(int argc, char **argv) : program(0)
{
    // Compile and link the shaders.

    if ((program = init_program(init_shader(GL_VERTEX_SHADER,   vert_src),
                                init_shader(GL_FRAGMENT_SHADER, frag_src))))
    {
        glUseProgram(program);

        // Query the program uniforms.

        ProjectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
        ModelViewMatrixLocation  = glGetUniformLocation(program, "ModelViewMatrix");
        NormalMatrixLocation     = glGetUniformLocation(program, "NormalMatrix");
        LightPositionLocation    = glGetUniformLocation(program, "LightPosition");
        AmbientLightLocation     = glGetUniformLocation(program, "AmbientLight");

        NormalTextureLocation    = glGetUniformLocation(program, "NormalTexture");
        DiffuseTextureLocation   = glGetUniformLocation(program, "DiffuseTexture");
        SpecularTextureLocation  = glGetUniformLocation(program, "SpecularTexture");

        // Query the vertex attributes.

        vTangentLocation         = glGetAttribLocation(program, "vTangent");
        vNormalLocation          = glGetAttribLocation(program, "vNormal");
        vTexCoordLocation        = glGetAttribLocation(program, "vTexCoord");
        vPositionLocation        = glGetAttribLocation(program, "vPosition");

        // Load each of the OBJ models named on the command line.

        float w = 0;
        float x = 0;
        float y = 0;
        float z = 0;

        for (int i = 1; i < argc; i++)
        {
            if (obj *o = obj_create(argv[i]))
            {
                // Configure the object's vertex attributes.

                obj_set_vert_loc(o, vTangentLocation,
                                    vNormalLocation,
                                    vTexCoordLocation,
                                    vPositionLocation);

                obj_set_prop_loc(o, OBJ_KN, -1, NormalTextureLocation,   -1);
                obj_set_prop_loc(o, OBJ_KD, -1, DiffuseTextureLocation,  -1);
                obj_set_prop_loc(o, OBJ_KS, -1, SpecularTextureLocation, -1);

                // Position the object at the origin on the XZ plane.

                float b[6];

                obj_bound(o, b);

                objects[o] = vec3(x - b[0], -b[1], -0.5 * (b[5] + b[2]));

                w = x + (b[3] - b[0]);
                x = x + (b[3] - b[0]) * 1.25;

                y = std::max(y, b[4] - b[1]);
                z = std::max(z, b[5] - b[2]);
            }
        }

        // Position the scene and default viewpoint.

        offset   = vec3(-0.5 * w, 0.0, 0.0);
        position = vec3(0.0, y, z);

        // Configure the lights.

        glUniform4f(AmbientLightLocation,  0.2, 0.2, 0.2, 1.0);
        glUniform4f(LightPositionLocation, 0.2, 0.1, 0.2, 0.0);

        glUseProgram(0);
    }

    // Set the necessary OpenGL state.

    glEnable(GL_DEPTH_TEST);
}


OVR_SDL2_obj::~OVR_SDL2_obj()
{
}

void OVR_SDL2_obj::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_TRUE, projection());

    for (auto i : objects)
    {
        mat4 M = view() * translation(offset) * translation(i.second);

        glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_TRUE, M);
        glUniformMatrix3fv(NormalMatrixLocation,    1, GL_TRUE, normal(M));

        obj_render(i.first);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

//------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    OVR_SDL2_obj app(argc, argv);
    app.run();
    return 0;
}

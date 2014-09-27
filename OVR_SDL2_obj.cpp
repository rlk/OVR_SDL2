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
    in vec2 vTexCoord;

    out vec3 fView;
    out vec3 fLight;
    out vec3 fNormal;
    out vec2 fTexCoord;

    void main()
    {
        fView     = vec3(ModelViewMatrix * vPosition);
        fLight    = vec3(ModelViewMatrix * LightPosition);
        fNormal   = vec3(NormalMatrix    * vNormal);
        fTexCoord = vTexCoord;

        gl_Position = ProjectionMatrix * ModelViewMatrix * vPosition;
    }
)";

const char *frag_src = R"(
    #version 150

    uniform vec3 AmbientLight;

    uniform vec4 DiffuseColor;
    uniform vec4 SpecularColor;

    uniform sampler2D DiffuseTexture;
    uniform sampler2D SpecularTexture;

    in vec3 fView;
    in vec3 fLight;
    in vec3 fNormal;
    in vec2 fTexCoord;

    out vec4 fOutput;

    void main()
    {
        // Sample the materials.

        vec3 cD = texture(DiffuseTexture,  fTexCoord).rgb + DiffuseColor.rgb;
        vec3 cS = texture(SpecularTexture, fTexCoord).rgb + SpecularColor.rgb;

        // Determine the per-fragment lighting vectors.

        vec3 N = normalize(fNormal);
        vec3 L = normalize(fLight);
        vec3 V = normalize(fView);
        vec3 R = reflect(L, N);

        // Compute the diffuse shading.

        float kd =     max(dot(L, N), 0.0);
        float ks = pow(max(dot(V, R), 0.0), 8.0);

        // Calculate the fragment color.

        fOutput = vec4(AmbientLight * cD + kd * cD + ks * cS, 1.0);
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

        // Query the program uniform locations.

        ProjectionMatrixLoc = glGetUniformLocation(program, "ProjectionMatrix");
        ModelViewMatrixLoc  = glGetUniformLocation(program, "ModelViewMatrix");
        NormalMatrixLoc     = glGetUniformLocation(program, "NormalMatrix");
        LightPositionLoc    = glGetUniformLocation(program, "LightPosition");
        AmbientLightLoc     = glGetUniformLocation(program, "AmbientLight");

        DiffuseColorLoc     = glGetUniformLocation(program, "DiffuseColor");
        SpecularColorLoc    = glGetUniformLocation(program, "SpecularColor");
        DiffuseTextureLoc   = glGetUniformLocation(program, "DiffuseTexture");
        SpecularTextureLoc  = glGetUniformLocation(program, "SpecularTexture");

        // Query the vertex attribute locations.

        vNormalLoc   = glGetAttribLocation(program, "vNormal");
        vTexCoordLoc = glGetAttribLocation(program, "vTexCoord");
        vPositionLoc = glGetAttribLocation(program, "vPosition");

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

                obj_set_vert_loc(o, -1, vNormalLoc,
                                        vTexCoordLoc,
                                        vPositionLoc);

                obj_set_prop_loc(o, OBJ_KD, DiffuseColorLoc,  DiffuseTextureLoc,  -1);
                obj_set_prop_loc(o, OBJ_KS, SpecularColorLoc, SpecularTextureLoc, -1);

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

        glUniform4f(AmbientLightLoc,  0.2, 0.2, 0.2, 1.0);
        glUniform4f(LightPositionLoc, 0.2, 0.1, 0.2, 0.0);

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

    glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_TRUE, projection());

    for (auto i : objects)
    {
        mat4 M = view() * translation(offset) * translation(i.second);

        glUniformMatrix4fv(ModelViewMatrixLoc, 1, GL_TRUE, M);
        glUniformMatrix3fv(NormalMatrixLoc,    1, GL_TRUE, normal(M));

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

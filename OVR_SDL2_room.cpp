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

#include <vector>

#include "OVR_SDL2_room.hpp"

//------------------------------------------------------------------------------

/// The scene is defined as a collection of 90 colored, axis-aligned boxes with
/// an XZ checkerboard pattern applied.

struct block
{
    float x0;  // Minimum X
    float y0;  // Minimum Y
    float z0;  // Minimum Z

    float x1;  // Maximum X
    float y1;  // Maximum Y
    float z1;  // Maximum Z

    float r;   // Base red
    float g;   // Base green
    float b;   // Base blue

    float s;   // Checker scale
    float f;   // Checker fraction
    float k;   // Checker brightness
};

struct block blocks[] = {
    { +1.80, -0.00, -2.71, +1.89, +0.09, +0.91, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, -0.00, -2.71, -1.80, +0.09, +0.91, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +1.62, -0.00, -2.80, +1.89, +0.09, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, -0.00, -2.80, +0.52, +0.09, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +0.42, +0.77, -0.61, +0.48, +0.79, -0.53, 0.8, 0.8, 0.8, 1.0, 1.00, 1.0 },
    { +0.29, +0.76, -0.72, +0.52, +0.77, -0.49, 0.3, 0.3, 1.0, 1.0, 1.00, 1.0 },
    { -0.08, +0.79, -0.93, +0.08, +1.02, -0.90, 0.8, 0.8, 0.8, 1.0, 1.00, 1.0 },
    { -0.15, +0.76, -0.98, +0.15, +0.79, -0.75, 0.8, 0.8, 0.8, 1.0, 1.00, 1.0 },
    { -0.21, +0.76, -0.67, +0.21, +0.78, -0.53, 0.8, 0.8, 0.8, 1.0, 1.00, 1.0 },
    { -0.33, +0.90, -0.91, +0.33, +1.36, -0.86, 0.8, 0.8, 0.8, 1.0, 1.00, 1.0 },
    { -1.89, +2.44, -2.80, +1.89, +2.50, +0.91, 0.9, 0.9, 0.9, 0.5, 0.05, 0.9 },
    { +1.80, +1.16, -1.13, +1.83, +1.22, -1.07, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.16, -0.15, +1.83, +1.22, -0.09, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.16, -1.07, +1.83, +1.22, -0.15, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.83, -1.13, +1.83, +1.89, -1.07, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.83, -0.15, +1.83, +1.89, -0.09, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.83, -1.07, +1.83, +1.89, -0.15, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.22, -1.13, +1.83, +1.83, -1.07, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.80, +1.22, -0.15, +1.83, +1.83, -0.09, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.81, +1.22, -1.07, +1.83, +1.83, -0.15, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.18, +0.34, -0.21, +0.18, +0.43, -0.19, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.18, +0.34, +0.19, +0.18, +0.43, +0.21, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.19, +0.34, -0.17, +0.21, +0.43, +0.17, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.21, +0.34, -0.17, -0.19, +0.43, +0.17, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.81, +0.64, -1.08, +0.83, +0.73, -0.37, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.83, +0.64, -1.08, -0.81, +0.73, -0.37, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.79, +0.64, -1.13, +0.79, +0.73, -1.10, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.79, +0.64, -0.35, +0.79, +0.73, -0.32, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.18, +0.70, +0.19, +0.18, +0.81, +0.21, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.18, +0.85, +0.19, +0.18, +0.96, +0.21, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.18, +0.46, +0.17, +0.22, +0.98, +0.23, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.22, +0.46, +0.17, -0.18, +0.98, +0.23, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.18, +0.00, -0.23, +0.22, +0.43, -0.17, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.22, +0.00, -0.23, -0.18, +0.43, -0.17, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.18, +0.00, +0.17, +0.22, +0.43, +0.23, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.22, +0.00, +0.17, -0.18, +0.43, +0.23, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { -0.25, +0.43, -0.25, +0.25, +0.46, +0.23, 0.9, 0.8, 0.4, 1.0, 1.00, 1.0 },
    { +0.79, +0.00, -1.15, +0.85, +0.73, -1.08, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.85, +0.00, -1.15, -0.79, +0.73, -1.08, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +0.79, +0.00, -0.37, +0.85, +0.73, -0.30, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.85, +0.00, -0.37, -0.79, +0.73, -0.30, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { -0.91, +0.73, -1.18, +0.91, +0.76, -0.27, 0.6, 0.5, 0.2, 1.0, 1.00, 1.0 },
    { +1.83, +0.09, -2.74, +1.89, +2.44, +0.91, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { +0.52, +2.15, -2.80, +1.62, +2.44, -2.74, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { +1.62, +0.09, -2.80, +1.89, +2.44, -2.74, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -2.80, +0.52, +2.44, -2.74, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +2.07, -2.38, -1.83, +2.44, -1.58, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +2.07, -1.31, -1.83, +2.44, -0.52, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +2.07, -0.24, -1.83, +2.44, +0.55, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -2.74, -1.83, +2.44, -2.38, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, +0.55, -1.83, +2.44, +0.91, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -1.58, -1.83, +2.44, -1.31, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -0.52, -1.83, +2.44, -0.24, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -2.38, -1.83, +0.52, -1.58, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -1.31, -1.83, +0.52, -0.52, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.89, +0.09, -0.24, -1.83, +0.52, +0.55, 0.4, 0.8, 0.6, 0.1, 0.50, 0.9 },
    { -1.85, +1.25, -0.15, -1.81, +1.28, +0.46, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.85, +1.25, -2.29, -1.81, +1.28, -1.68, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.85, +1.25, -1.22, -1.81, +1.28, -0.61, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -0.24, -1.71, +0.61, -0.15, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, +0.46, -1.71, +0.61, +0.55, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -0.15, -1.71, +0.61, +0.46, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -0.24, -1.77, +2.07, -0.15, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, +0.46, -1.77, +2.07, +0.55, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -0.15, -1.77, +2.07, +0.46, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, -0.24, -1.77, +1.98, -0.15, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, +0.46, -1.77, +1.98, +0.55, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -2.38, -1.71, +0.61, -2.29, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -1.68, -1.71, +0.61, -1.58, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -2.29, -1.71, +0.61, -1.68, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -2.38, -1.77, +2.07, -2.29, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -1.68, -1.77, +2.07, -1.58, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -2.29, -1.77, +2.07, -1.68, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, -2.38, -1.77, +1.98, -2.29, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, -1.68, -1.77, +1.98, -1.58, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -1.31, -1.71, +0.61, -1.22, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -0.61, -1.71, +0.61, -0.52, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.52, -1.22, -1.71, +0.61, -0.61, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -1.31, -1.77, +2.07, -1.22, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -0.61, -1.77, +2.07, -0.52, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +1.98, -1.22, -1.77, +2.07, -0.61, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, -1.31, -1.77, +1.98, -1.22, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { -1.89, +0.61, -0.61, -1.77, +1.98, -0.52, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +1.52, +2.06, -2.80, +1.62, +2.15, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +0.52, +2.06, -2.80, +0.61, +2.15, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +0.61, +2.06, -2.80, +1.52, +2.15, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +1.52, -0.00, -2.80, +1.62, +2.06, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +0.52, -0.00, -2.80, +0.61, +2.06, -2.71, 0.9, 0.9, 0.9, 1.0, 1.00, 1.0 },
    { +0.61, +0.00, -2.80, +1.52, +2.06, -2.74, 0.8, 0.0, 0.0, 1.0, 1.00, 1.0 },
    { -1.89, -0.06, -2.80, +1.89, +0.00, +0.91, 0.7, 0.7, 0.7, 0.5, 0.50, 0.8 },
};

static const char *vert_src = R"(
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
)";

static const char *frag_src = R"(
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
        vec3  L = normalize(LightPosition.xyz - fPosition.xyz);
        vec3  N = normalize(fNormal);

        vec3  a = fColor;
        vec3  b = fColor * fChecker.z;

        vec3  c = step(fChecker.y, fract(fCoord / fChecker.x));
        float d = max(dot(L, N), 0.0);

        float k = c.x + c.z - 2.0 * c.x * c.z;

        fOutput = vec4(mix(a, b, k) * (d + AmbientLight), 1.0);
    }
)";

//------------------------------------------------------------------------------

/// Triangulate and append the vertices of a block face onto the vertex array.
///
/// @param p0     Position vector
/// @param p1     Position vector
/// @param p2     Position vector
/// @param p3     Position vector
/// @param n      Normal vector
/// @param c      Color
/// @param x      Checker parameters

void OVR_SDL2_room::make_face(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec3 n, vec3 c, vec3 x)
{
    verts.push_back(vert(p0, n, c, x));
    verts.push_back(vert(p1, n, c, x));
    verts.push_back(vert(p2, n, c, x));

    verts.push_back(vert(p0, n, c, x));
    verts.push_back(vert(p2, n, c, x));
    verts.push_back(vert(p3, n, c, x));
}

/// Append the vertices of an axis-aligned block onto the vertex array.
///
/// @param p0     Minimum position vector
/// @param p1     Maximum position vector
/// @param c      Color
/// @param x      Checker parameters

void OVR_SDL2_room::make_block(vec3 p0, vec3 p1, vec3 c, vec3 x)
{
    make_face(vec3(p1[0], p1[1], p1[2]),
              vec3(p1[0], p0[1], p1[2]),
              vec3(p1[0], p0[1], p0[2]),
              vec3(p1[0], p1[1], p0[2]), vec3(+1, 0, 0), c, x);

    make_face(vec3(p0[0], p1[1], p0[2]),
              vec3(p0[0], p0[1], p0[2]),
              vec3(p0[0], p0[1], p1[2]),
              vec3(p0[0], p1[1], p1[2]), vec3(-1, 0, 0), c, x);

    make_face(vec3(p0[0], p1[1], p0[2]),
              vec3(p0[0], p1[1], p1[2]),
              vec3(p1[0], p1[1], p1[2]),
              vec3(p1[0], p1[1], p0[2]), vec3(0, +1, 0), c, x);

    make_face(vec3(p0[0], p0[1], p1[2]),
              vec3(p0[0], p0[1], p0[2]),
              vec3(p1[0], p0[1], p0[2]),
              vec3(p1[0], p0[1], p1[2]), vec3(0, -1, 0), c, x);

    make_face(vec3(p0[0], p1[1], p1[2]),
              vec3(p0[0], p0[1], p1[2]),
              vec3(p1[0], p0[1], p1[2]),
              vec3(p1[0], p1[1], p1[2]), vec3(0, 0, +1), c, x);

    make_face(vec3(p1[0], p1[1], p0[2]),
              vec3(p1[0], p0[1], p0[2]),
              vec3(p0[0], p0[1], p0[2]),
              vec3(p0[0], p1[1], p0[2]), vec3(0, 0, -1), c, x);
}

//------------------------------------------------------------------------------

/// Initialize the room demo.

OVR_SDL2_room::OVR_SDL2_room()
{
    const size_t stride = sizeof (vert);

    // Converting the array of colored blocks into a list of triangles.

    for (size_t i = 0; i < sizeof (blocks) / sizeof (blocks[0]); i++)

        make_block(vec3(blocks[i].x0, blocks[i].y0, blocks[i].z0),
                   vec3(blocks[i].x1, blocks[i].y1, blocks[i].z1),
                   vec3(blocks[i].r,  blocks[i].g,  blocks[i].b),
                   vec3(blocks[i].s,  blocks[i].f,  blocks[i].k));

    // Generate a vertex array and buffer object to hold these triangles.

    glGenVertexArrays(1, &vao);
    glBindVertexArray(    vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * stride,
                                 &verts.front(), GL_STATIC_DRAW);

    // Initialize a shader to render the vertex array.

    if ((program = init_program(init_shader(GL_VERTEX_SHADER,   vert_src),
                                init_shader(GL_FRAGMENT_SHADER, frag_src))))
    {
        glUseProgram(program);

        ProjectionMatrixLoc = glGetUniformLocation(program, "ProjectionMatrix");
        ModelViewMatrixLoc  = glGetUniformLocation(program, "ModelViewMatrix");
        NormalMatrixLoc     = glGetUniformLocation(program, "NormalMatrix");
        LightPositionLoc    = glGetUniformLocation(program, "LightPosition");
        AmbientLightLoc     = glGetUniformLocation(program, "AmbientLight");

        GLint pLoc = glGetAttribLocation(program, "vPosition");
        GLint nLoc = glGetAttribLocation(program, "vNormal");
        GLint cLoc = glGetAttribLocation(program, "vColor");
        GLint xLoc = glGetAttribLocation(program, "vChecker");

        glEnableVertexAttribArray(pLoc);
        glEnableVertexAttribArray(nLoc);
        glEnableVertexAttribArray(cLoc);
        glEnableVertexAttribArray(xLoc);

        glVertexAttribPointer(pLoc, 3, GL_FLOAT, 0, stride, (const void *)  0);
        glVertexAttribPointer(nLoc, 3, GL_FLOAT, 0, stride, (const void *) 12);
        glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, stride, (const void *) 24);
        glVertexAttribPointer(xLoc, 3, GL_FLOAT, 0, stride, (const void *) 36);

        glUseProgram(0);
    }
    glBindVertexArray(0);

    position = vec3(0.0f, 1.3f, 0.0f);
    ambient  = vec3(0.2f, 0.2f, 0.2f);
    light    = vec4(0.0f, 2.1f, 0.0f, 1.0f);

    glClearColor(0.3f, 0.6f, 0.9f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

OVR_SDL2_room::~OVR_SDL2_room()
{
    glDeleteProgram(program);

    glDeleteBuffers     (1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void OVR_SDL2_room::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);

    mat4 P = projection();
    mat4 V = view();

    glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_TRUE, P);
    glUniformMatrix4fv(ModelViewMatrixLoc,  1, GL_TRUE, V);
    glUniformMatrix3fv(NormalMatrixLoc,     1, GL_TRUE, normal(V));
    glUniform4fv      (LightPositionLoc,    1,          V * light);
    glUniform3fv      (AmbientLightLoc,     1,          ambient);

    glDrawArrays(GL_TRIANGLES, 0, verts.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

//------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    OVR_SDL2_room app;
    app.run();
    return 0;
}


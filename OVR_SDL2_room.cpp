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
    { +5.90, -0.00, -8.90, +6.20, +0.30, +3.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, -0.00, -8.90, -5.90, +0.30, +3.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +5.30, -0.00, -9.20, +6.20, +0.30, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, -0.00, -9.20, +1.70, +0.30, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +1.34, +2.50, -2.73, +1.53, +2.57, -2.45, 0.8, 0.8, 0.8, 1.0, 0.00, 1.0 },
    { +0.95, +2.49, -2.97, +1.70, +2.51, -2.22, 0.3, 0.3, 1.0, 1.0, 0.00, 1.0 },
    { -0.27, +2.60, -3.67, +0.27, +3.35, -3.57, 0.8, 0.8, 0.8, 1.0, 0.00, 1.0 },
    { -0.50, +2.50, -3.85, +0.50, +2.60, -3.10, 0.8, 0.8, 0.8, 1.0, 0.00, 1.0 },
    { -0.69, +2.50, -2.82, +0.69, +2.55, -2.36, 0.8, 0.8, 0.8, 1.0, 0.00, 1.0 },
    { -1.07, +2.96, -3.61, +1.07, +4.47, -3.45, 0.8, 0.8, 0.8, 1.0, 0.00, 1.0 },
    { -6.20, +8.00, -9.20, +6.20, +8.20, +3.00, 0.9, 0.9, 0.9, 0.5, 0.05, 1.1 },
    { +5.90, +3.80, -3.70, +6.00, +4.00, -3.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +3.80, -0.50, +6.00, +4.00, -0.30, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +3.80, -3.50, +6.00, +4.00, -0.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +6.00, -3.70, +6.00, +6.20, -3.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +6.00, -0.50, +6.00, +6.20, -0.30, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +6.00, -3.50, +6.00, +6.20, -0.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +4.00, -3.70, +6.00, +6.00, -3.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.90, +4.00, -0.50, +6.00, +6.00, -0.30, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +5.95, +4.00, -3.50, +6.00, +6.00, -0.50, 0.9, 0.7, 0.3, 1.0, 0.00, 1.0 },
    { -0.57, +1.10, -0.69, +0.57, +1.40, -0.63, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.57, +1.10, +0.63, +0.57, +1.40, +0.69, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +0.63, +1.10, -0.57, +0.69, +1.40, +0.57, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.69, +1.10, -0.57, -0.63, +1.40, +0.57, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +2.65, +2.00, -4.13, +2.73, +2.30, -1.87, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -2.73, +2.00, -4.13, -2.65, +2.30, -1.87, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -2.55, +2.00, -4.32, +2.55, +2.30, -4.23, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -2.55, +2.00, -1.77, +2.55, +2.30, -1.68, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -0.57, +2.30, +0.63, +0.57, +2.66, +0.68, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.57, +2.78, +0.63, +0.57, +3.15, +0.68, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +0.57, +1.50, +0.57, +0.75, +3.22, +0.75, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.75, +1.50, +0.57, -0.57, +3.22, +0.75, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +0.57, +0.00, -0.75, +0.75, +1.40, -0.57, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.75, +0.00, -0.75, -0.57, +1.40, -0.57, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +0.57, +0.00, +0.57, +0.75, +1.40, +0.75, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.75, +0.00, +0.57, -0.57, +1.40, +0.75, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { -0.81, +1.40, -0.81, +0.81, +1.50, +0.75, 0.9, 0.8, 0.4, 1.0, 0.00, 1.0 },
    { +2.55, +0.00, -4.42, +2.83, +2.30, -4.13, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -2.83, +0.00, -4.42, -2.55, +2.30, -4.13, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +2.55, +0.00, -1.87, +2.83, +2.30, -1.58, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -2.83, +0.00, -1.87, -2.55, +2.30, -1.58, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { -3.00, +2.30, -4.50, +3.00, +2.50, -1.50, 0.6, 0.5, 0.2, 1.0, 0.00, 1.0 },
    { +6.00, +0.30, -9.00, +6.20, +8.00, +3.00, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { +1.70, +7.05, -9.20, +5.30, +8.00, -9.00, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { +5.30, +0.30, -9.20, +6.20, +8.00, -9.00, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -9.20, +1.70, +8.00, -9.00, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +6.80, -7.80, -6.00, +8.00, -5.20, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +6.80, -4.30, -6.00, +8.00, -1.70, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +6.80, -0.80, -6.00, +8.00, +1.80, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -9.00, -6.00, +8.00, -7.80, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, +1.80, -6.00, +8.00, +3.00, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -5.20, -6.00, +8.00, -4.30, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -1.70, -6.00, +8.00, -0.80, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -7.80, -6.00, +1.70, -5.20, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -4.30, -6.00, +1.70, -1.70, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.20, +0.30, -0.80, -6.00, +1.70, +1.80, 0.3, 0.7, 0.5, 2.0, 0.50, 0.9 },
    { -6.07, +4.10, -0.50, -5.93, +4.20, +1.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.07, +4.10, -7.50, -5.93, +4.20, -5.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.07, +4.10, -4.00, -5.93, +4.20, -2.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -0.80, -5.63, +2.00, -0.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, +1.50, -5.63, +2.00, +1.80, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -0.50, -5.63, +2.00, +1.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -0.80, -5.80, +6.80, -0.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, +1.50, -5.80, +6.80, +1.80, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -0.50, -5.80, +6.80, +1.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, -0.80, -5.80, +6.50, -0.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, +1.50, -5.80, +6.50, +1.80, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -7.80, -5.63, +2.00, -7.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -5.50, -5.63, +2.00, -5.20, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -7.50, -5.63, +2.00, -5.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -7.80, -5.80, +6.80, -7.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -5.50, -5.80, +6.80, -5.20, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -7.50, -5.80, +6.80, -5.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, -7.80, -5.80, +6.50, -7.50, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, -5.50, -5.80, +6.50, -5.20, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -4.30, -5.63, +2.00, -4.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -2.00, -5.63, +2.00, -1.70, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +1.70, -4.00, -5.63, +2.00, -2.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -4.30, -5.80, +6.80, -4.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -2.00, -5.80, +6.80, -1.70, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +6.50, -4.00, -5.80, +6.80, -2.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, -4.30, -5.80, +6.50, -4.00, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { -6.20, +2.00, -2.00, -5.80, +6.50, -1.70, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +5.00, +6.75, -9.20, +5.30, +7.05, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +1.70, +6.75, -9.20, +2.00, +7.05, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +2.00, +6.75, -9.20, +5.00, +7.05, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +5.00, -0.00, -9.20, +5.30, +6.75, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +1.70, -0.00, -9.20, +2.00, +6.75, -8.90, 0.9, 0.9, 0.9, 1.0, 0.00, 1.0 },
    { +2.00, +0.00, -9.20, +5.00, +6.75, -9.00, 0.8, 0.0, 0.0, 1.0, 0.00, 1.0 },
    { -6.20, -0.20, -9.20, +6.20, +0.00, +3.00, 0.6, 0.6, 0.6, 0.5, 0.50, 0.8 },
};

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

    for (int i = 0; i < sizeof (blocks) / sizeof (blocks[0]); i++)

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

    if ((program = init_program("vertex.glsl", "fragment.glsl")))
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
    }

    position = vec3(0.0, 4.0, 0.0);
    ambient  = vec3(0.3, 0.3, 0.3);
    light    = vec4(0.0, 7.0, 0.0, 1.0);

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
    mat4 P = projection();
    mat4 V = view();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_TRUE, P);
    glUniformMatrix4fv(ModelViewMatrixLoc,  1, GL_TRUE, V);
    glUniformMatrix3fv(NormalMatrixLoc,     1, GL_TRUE, normal(V));
    glUniform4fv      (LightPositionLoc,    1,          V * light);
    glUniform3fv      (AmbientLightLoc,     1,          ambient);

    glDrawArrays(GL_TRIANGLES, 0, verts.size());
}

//------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    OVR_SDL2_room app;
    app.run();
    return 0;
}


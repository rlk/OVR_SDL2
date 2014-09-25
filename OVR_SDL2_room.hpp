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

#ifndef OVR_SDL2_ROOM_HPP
#define OVR_SDL2_ROOM_HPP

#include "OVR_SDL2_nav.hpp"

//------------------------------------------------------------------------------

class OVR_SDL2_room : public OVR_SDL2_nav
{
public:

    OVR_SDL2_room();

    virtual ~OVR_SDL2_room();

protected:

    virtual void draw();

private:

    struct vert
    {
        vec3 p;
        vec3 n;
        vec3 c;
        vec3 x;
        vert(vec3 p, vec3 n, vec3 c, vec3 x) : p(p), n(n), c(c), x(x) { }
    };

    void make_face (vec3, vec3, vec3, vec3, vec3, vec3, vec3);
    void make_block(vec3, vec3, vec3, vec3);

    std::vector<vert> verts;

    GLuint vao;
    GLuint vbo;
    GLuint program;

    GLint ProjectionMatrixLoc;
    GLint ModelViewMatrixLoc;
    GLint NormalMatrixLoc;
    GLint LightPositionLoc;
    GLint AmbientLightLoc;

    vec3 ambient;
    vec4 light;
};

//------------------------------------------------------------------------------

#endif

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

#ifndef OVR_SDL2_NAV_HPP
#define OVR_SDL2_NAV_HPP

#include "OVR_SDL2_app.hpp"

//------------------------------------------------------------------------------

class OVR_SDL2_nav : public OVR_SDL2_app
{
public:

    OVR_SDL2_nav();

    virtual ~OVR_SDL2_nav();

protected:

    virtual void step();

    virtual void keyboard    (int, bool, bool);
    virtual void mouse_motion(int, int);
    virtual void game_button (int, int, bool);
    virtual void game_axis   (int, int, float);

    virtual mat4 view() const;

    vec3  position;
    float rotation;

private:

    vec3  dposition;
    float drotation;

    bool  move_L;
    bool  move_R;
    bool  move_D;
    bool  move_U;
    bool  move_F;
    bool  move_B;
};

//------------------------------------------------------------------------------

#endif

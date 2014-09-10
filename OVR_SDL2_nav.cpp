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

#include "OVR_SDL2_nav.hpp"

//------------------------------------------------------------------------------

OVR_SDL2_nav::OVR_SDL2_nav() : rotation(0)
{
}

OVR_SDL2_nav::~OVR_SDL2_nav()
{
}

/// Animate the camera position.

void OVR_SDL2_nav::step()
{
    gl::mat3 N = gl::normal(gl::inverse(view()));
    position = position + N * velocity / 30.0;
}

/// Handle a key press or release.

void OVR_SDL2_nav::keyboard(int key, bool down, bool repeat)
{
    if (!repeat)
    {
        if (down)
            switch (key)
            {
            case SDL_SCANCODE_A:     velocity[0] -= 1.0; return;
            case SDL_SCANCODE_D:     velocity[0] += 1.0; return;
            case SDL_SCANCODE_C:     velocity[1] -= 1.0; return;
            case SDL_SCANCODE_SPACE: velocity[1] += 1.0; return;
            case SDL_SCANCODE_W:     velocity[2] -= 1.0; return;
            case SDL_SCANCODE_S:     velocity[2] += 1.0; return;
            }
        else
            switch (key)
            {
            case SDL_SCANCODE_A:     velocity[0] += 1.0; return;
            case SDL_SCANCODE_D:     velocity[0] -= 1.0; return;
            case SDL_SCANCODE_C:     velocity[1] += 1.0; return;
            case SDL_SCANCODE_SPACE: velocity[1] -= 1.0; return;
            case SDL_SCANCODE_W:     velocity[2] += 1.0; return;
            case SDL_SCANCODE_S:     velocity[2] -= 1.0; return;
            }
    }
    OVR_SDL2_app::keyboard(key, down, repeat);
}

/// Handle a mouse button press or release.

void OVR_SDL2_nav::mouse_button(int button, bool down)
{
}

/// Handle mouse wheel rotation.

void OVR_SDL2_nav::mouse_wheel(int dx, int dy)
{
}

/// Handle mouse pointer motion.

void OVR_SDL2_nav::mouse_motion(int dx, int dy)
{
    rotation += dx;
}

/// Handle gamepad button press or release.

void OVR_SDL2_nav::game_button(int button, bool down)
{
}

/// Handle gamepad axis motion.

void OVR_SDL2_nav::game_axis(int axis, float value)
{
}

/// Return the current view matrix.

gl::mat4 OVR_SDL2_nav::view() const
{
    return gl::xrotation(gl::to_radians(rotation)) * gl::translation(-position);
}

//------------------------------------------------------------------------------

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

OVR_SDL2_nav::OVR_SDL2_nav() :
    rotation(0), drotation(0),
    move_L(false),
    move_R(false),
    move_D(false),
    move_U(false),
    move_F(false),
    move_B(false)
{
    // FPS-style mouselook requires relative mouse motion.

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

OVR_SDL2_nav::~OVR_SDL2_nav()
{
}

/// Animate the camera position.

void OVR_SDL2_nav::step()
{
    mat3 N = normal(inverse(yrotation(to_radians(rotation))));
    vec3 v = dposition;

    if (move_L) v = v + vec3(-1, 0, 0);
    if (move_R) v = v + vec3(+1, 0, 0);
    if (move_D) v = v + vec3(0, -1, 0);
    if (move_U) v = v + vec3(0, +1, 0);
    if (move_F) v = v + vec3(0, 0, -1);
    if (move_B) v = v + vec3(0, 0, +1);

    if (length(v) > 1.0)
        v = normalize(v);

    position = position + N * v / 30.0;
    rotation = rotation + drotation;
}

/// Handle a key press or release.

void OVR_SDL2_nav::keyboard(int key, bool down, bool repeat)
{
    OVR_SDL2_app::keyboard(key, down, repeat);

    if (!repeat)
    {
        switch (key)
        {
            case SDL_SCANCODE_A:     move_L = down; return;
            case SDL_SCANCODE_D:     move_R = down; return;
            case SDL_SCANCODE_C:     move_D = down; return;
            case SDL_SCANCODE_SPACE: move_U = down; return;
            case SDL_SCANCODE_W:     move_F = down; return;
            case SDL_SCANCODE_S:     move_B = down; return;
        }
    }
}

/// Handle mouse pointer motion.

void OVR_SDL2_nav::mouse_motion(int dx, int dy)
{
    rotation += 0.5f * dx;
}

/// Handle gamepad button press or release.

void OVR_SDL2_nav::game_button(int device, int button, bool down)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:  move_L = down; break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: move_R = down; break;
        case SDL_CONTROLLER_BUTTON_B:          move_D = down; break;
        case SDL_CONTROLLER_BUTTON_A:          move_U = down; break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:    move_F = down; break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:  move_B = down; break;
    }
}

/// Handle gamepad axis motion.

void OVR_SDL2_nav::game_axis(int device, int axis, float value)
{
    float k = copysignf(value * value, value);

    switch (axis)
    {
        case SDL_CONTROLLER_AXIS_LEFTX:
            dposition[0] = (fabsf(k) > 0.1f) ? k : 0.0f;
            break;
        case SDL_CONTROLLER_AXIS_LEFTY:
            dposition[2] = (fabsf(k) > 0.1f) ? k : 0.0f;
            break;
        case SDL_CONTROLLER_AXIS_RIGHTX:
            drotation    = (fabsf(k) > 0.1f) ? k : 0.0f;
            break;
    }
}

//------------------------------------------------------------------------------

/// Return the current view matrix.

mat4 OVR_SDL2_nav::view() const
{
    return OVR_SDL2_app::view() * yrotation(to_radians(rotation))
                                * translation(-position);
}

//------------------------------------------------------------------------------

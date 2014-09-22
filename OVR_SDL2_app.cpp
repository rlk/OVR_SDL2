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

#include "OVR_SDL2_app.hpp"

//------------------------------------------------------------------------------

/// Initialize an SDL OpenGL window.

OVR_SDL2_app::OVR_SDL2_app()
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        int f = SDL_WINDOW_OPENGL;

        int w = 960;
        int h = 540;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        if ((window = SDL_CreateWindow("OVR_SDL2", x, y, w, h, f)))
        {
            context = SDL_GL_CreateContext(window);
            running = true;

            SDL_SetRelativeMouseMode(SDL_TRUE);

#ifdef GLEW_VERSION
            glewExperimental = GL_TRUE;
            glewInit();
#endif
        }
    }
}

/// Finalize the SDL OpenGL window.

OVR_SDL2_app::~OVR_SDL2_app()
{
    if (context) SDL_GL_DeleteContext(context);
    if (window)  SDL_DestroyWindow(window);
}

//------------------------------------------------------------------------------

/// Application main loop.

void OVR_SDL2_app::run()
{
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
            dispatch(e);

        step();
        draw();
        swap();
    }
}

/// Dispatch an SDL event.

void OVR_SDL2_app::dispatch(SDL_Event& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
            keyboard(e.key.keysym.scancode, true,  e.key.repeat); break;
        case SDL_KEYUP:
            keyboard(e.key.keysym.scancode, false, e.key.repeat); break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_button(e.button.button, true);  break;
        case SDL_MOUSEBUTTONUP:
            mouse_button(e.button.button, false); break;
        case SDL_MOUSEMOTION:
            mouse_motion(e.motion.xrel, e.motion.yrel); break;
        case SDL_MOUSEWHEEL:
            mouse_wheel(e.wheel.x, e.wheel.y); break;
        case SDL_JOYAXISMOTION:
            game_axis(e.jaxis.axis, e.jaxis.value / 32768.f); break;
        case SDL_JOYBUTTONDOWN:
            game_button(e.jbutton.button, true);  break;
        case SDL_JOYBUTTONUP:
            game_button(e.jbutton.button, false); break;
        case SDL_QUIT:
            running = false; break;
    }
}

//------------------------------------------------------------------------------

/// Draw the scene.

void OVR_SDL2_app::draw()
{
}

/// Swap buffers.

void OVR_SDL2_app::swap()
{
    SDL_GL_SwapWindow(window);
}

/// Animate.

void OVR_SDL2_app::step()
{
}

/// Handle a key press or release.

void OVR_SDL2_app::keyboard(int key, bool down, bool repeat)
{
    if (key == SDL_SCANCODE_ESCAPE && down == false)
        running = false;
}

/// Handle a mouse button press or release.

void OVR_SDL2_app::mouse_button(int button, bool down)
{
}

/// Handle mouse wheel rotation.

void OVR_SDL2_app::mouse_wheel(int dx, int dy)
{
}

/// Handle mouse pointer motion.

void OVR_SDL2_app::mouse_motion(int x, int y)
{
}

/// Handle gamepad button press or release.

void OVR_SDL2_app::game_button(int button, bool down)
{
}

/// Handle gamepad axis motion.

void OVR_SDL2_app::game_axis(int axis, float value)
{
}

//------------------------------------------------------------------------------

/// Return the current projection matrix.

mat4 OVR_SDL2_app::projection() const
{
    int w;
    int h;

    SDL_GetWindowSize(window, &w, &h);

    return perspective(to_radians(45.0), float(w) / float(h), 0.1, 100.0);
}

/// Return the current view matrix.

mat4 OVR_SDL2_app::view() const
{
    return mat4();
}

//------------------------------------------------------------------------------

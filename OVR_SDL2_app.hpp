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

#ifndef OVR_SDL2_APP_HPP
#define OVR_SDL2_APP_HPP

#include <vector>

#undef static_assert
#include <OVR.h>
#include <OVR_CAPI_GL.h>

#include <SDL2/SDL.h>

#include "GLFundamentals.hpp"
#include "GLFramebuffer.hpp"

using namespace gl;

//------------------------------------------------------------------------------

class OVR_SDL2_app
{
public:

    OVR_SDL2_app();

    void run();

    virtual ~OVR_SDL2_app();

protected:

    void dispatch(SDL_Event&);

    virtual void draw() { }
    virtual void step() { }

    virtual void keyboard    (int, bool, bool);
    virtual void mouse_button(int, bool);
    virtual void mouse_motion(int, int);
    virtual void mouse_wheel (int, int);
    virtual void game_connect(int, bool);
    virtual void game_button (int, int, bool);
    virtual void game_axis   (int, int, float);

    virtual mat4 projection() const;
    virtual mat4 view()       const;

    bool running;

private:

    bool init_SDL(int, int, int, int);
    bool init_OVR();
    void conf_OVR();

    ovrHmd            hmd;
    ovrEyeType        eye;
    ovrPosef         pose[2];
    ovrEyeRenderDesc  erd[2];
    ovrTexture        tex[2];
    framebuffer   *buffer[2];
    SDL_Window    *window;
    SDL_GLContext context;


    std::vector<SDL_GameController *> controller;
};

//------------------------------------------------------------------------------

#endif

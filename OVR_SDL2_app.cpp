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

/// Initialize an OVR HMD and SDL2 window.

OVR_SDL2_app::OVR_SDL2_app() : hmd(0), eye(ovrEye_Left), window(0)
{
    if (init_OVR())
    {
        running = init_SDL(hmd->WindowsPos.x,
                           hmd->WindowsPos.y,
                           hmd->Resolution.w,
                           hmd->Resolution.h);
        conf_OVR();
    }
}

/// Finalize the SDL and OVR.

OVR_SDL2_app::~OVR_SDL2_app()
{
    if (buffer[1]) delete buffer[1];
    if (buffer[0]) delete buffer[0];

    if (context) SDL_GL_DeleteContext(context);
    if (window)  SDL_DestroyWindow(window);

    if (hmd) ovrHmd_Destroy(hmd);

    ovr_Shutdown();
}

//------------------------------------------------------------------------------

/// Initialize an SDL window and GL context with the given position and size.

bool OVR_SDL2_app::init_SDL(int x, int y, int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) == 0)
    {
        int f = SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL;

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

            SDL_GameControllerEventState(SDL_ENABLE);
            SDL_SetRelativeMouseMode(SDL_TRUE);

            #ifdef GLEW_VERSION
            glewExperimental = GL_TRUE;
            glewInit();
            #endif

            return true;
        }
    }
    return false;
}

/// Initialize the Oculus SDK.
///
/// Enable all tracking capability and fall back on a DK1 debug configuration
/// if no HMD is available.

bool OVR_SDL2_app::init_OVR()
{
    ovr_Initialize();

    hmd = ovrHmd_Create(0);

    if (hmd == 0)
        hmd = ovrHmd_CreateDebug(ovrHmd_DK1);

    if (hmd)
        ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
                                      ovrTrackingCap_MagYawCorrection |
                                      ovrTrackingCap_Position, 0);
    return true;
}

/// Configure the Oculus SDK renderer.
///
/// This requires the presence of OpenGL state, so it much be performed AFTER
/// the SDL window has been created.

void OVR_SDL2_app::conf_OVR()
{
    // Configure the renderer with all features.

    ovrGLConfig cfg;

    memset(&cfg, 0, sizeof (ovrGLConfig));

    cfg.OGL.Header.API      = ovrRenderAPI_OpenGL;
    cfg.OGL.Header.RTSize.w = hmd->Resolution.w;
    cfg.OGL.Header.RTSize.h = hmd->Resolution.h;

    ovrHmd_ConfigureRendering(hmd, &cfg.Config, ovrDistortionCap_Chromatic
                                              | ovrDistortionCap_TimeWarp
                                              | ovrDistortionCap_Overdrive,
                                                hmd->DefaultEyeFov, erd);

    // Initialize the off-screen render buffers.

    ovrSizei size[2];

    size[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,  hmd->DefaultEyeFov[0], 1);
    size[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1);

    for (int i = 0; i < 2; i++)
    {
        if ((buffer[i] = new framebuffer(size[i].w, size[i].h)))
        {
            ovrGLTexture *p = reinterpret_cast<ovrGLTexture*>(tex + i);

            p->OGL.Header.API                   = ovrRenderAPI_OpenGL;
            p->OGL.Header.TextureSize           = size[i];
            p->OGL.Header.RenderViewport.Size   = size[i];
            p->OGL.Header.RenderViewport.Pos.x  = 0;
            p->OGL.Header.RenderViewport.Pos.y  = 0;
            p->OGL.TexId                        = buffer[i]->color;
        }
    }
}

//------------------------------------------------------------------------------

/// Application main loop.

void OVR_SDL2_app::run()
{
    SDL_Event e;

    while (running)
    {
        // Dispatch all pending SDL events.

        while (SDL_PollEvent(&e))
            dispatch(e);

        // Let the application animate.

        step();

        // Render both views. Let the Oculus SDK display them on-screen.

        ovrHmd_BeginFrame(hmd, 0);
        {
            for (int i = 0; i < 2; i++)
            {
                eye = hmd->EyeRenderOrder[i];
                pose[eye] = ovrHmd_GetEyePose(hmd, eye);
                buffer[eye]->bind();
                draw();
            }
        }
        ovrHmd_EndFrame(hmd, pose, tex);
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
        case SDL_CONTROLLERAXISMOTION:
            game_axis(e.caxis.which, e.caxis.axis, e.caxis.value / 32768.f); break;
        case SDL_CONTROLLERBUTTONDOWN:
            game_button(e.caxis.which, e.cbutton.button, true);  break;
        case SDL_CONTROLLERBUTTONUP:
            game_button(e.caxis.which, e.cbutton.button, false); break;
        case SDL_CONTROLLERDEVICEADDED:
            game_connect(e.cdevice.which, true);  break;
        case SDL_CONTROLLERDEVICEREMOVED:
            game_connect(e.cdevice.which, false); break;
        case SDL_QUIT:
            running = false; break;
    }
}

//------------------------------------------------------------------------------

/// Handle a key press or release.

void OVR_SDL2_app::keyboard(int key, bool down, bool repeat)
{
    ovrHSWDisplayState warning;

    ovrHmd_GetHSWDisplayState(hmd, &warning);

    if (warning.Displayed)
        ovrHmd_DismissHSWDisplay(hmd);

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

/// Handle gamepad connection or disconnection

void OVR_SDL2_app::game_connect(int device, bool connected)
{
    controller.resize(device + 1);

    if (controller[device])
        SDL_GameControllerClose(controller[device]);

    if (connected)
        controller[device] = SDL_GameControllerOpen(device);
    else
        controller[device] = 0;
}

/// Handle gamepad button press or release.

void OVR_SDL2_app::game_button(int device, int button, bool down)
{
}

/// Handle gamepad axis motion.

void OVR_SDL2_app::game_axis(int device, int axis, float value)
{
}

//------------------------------------------------------------------------------

/// Convert an OVR matrix to a GLFundamentals matrix.

static mat4 getMatrix4f(const OVR::Matrix4f& m)
{
    return mat4(m.M[0][0], m.M[0][1], m.M[0][2], m.M[0][3],
                m.M[1][0], m.M[1][1], m.M[1][2], m.M[1][3],
                m.M[2][0], m.M[2][1], m.M[2][2], m.M[2][3],
                m.M[3][0], m.M[3][1], m.M[3][2], m.M[3][3]);
}

/// Return the current projection matrix.

mat4 OVR_SDL2_app::projection() const
{
    return getMatrix4f(ovrMatrix4f_Projection(erd[eye].Fov, 0.1f, 100.0f, true));
}

/// Return the current view matrix.

mat4 OVR_SDL2_app::view() const
{
    OVR::Quatf q = OVR::Quatf(pose[eye].Orientation);

    mat4 O = getMatrix4f(OVR::Matrix4f(q.Inverted()));

    mat4 E = translation(vec3(erd[eye].ViewAdjust.x,
                              erd[eye].ViewAdjust.y,
                              erd[eye].ViewAdjust.z));

    mat4 H = translation(vec3(-pose[eye].Position.x,
                              -pose[eye].Position.y,
                              -pose[eye].Position.z));
    return E * O * H;
}

//------------------------------------------------------------------------------

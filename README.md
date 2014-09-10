# Oculus SDL2 OBJ Viewer

This is an SDL2-based cross-platform example application for the Oculus Rift written in C++. It uses an OpenGL 3.2 Core Profile and the Oculus VR SDK 0.4 to render Wavefront OBJ models to an Oculus Rift DK1 or DK2. Its modular design enables easy reuse and its license permits you to copy it for any purpose.

## Structure

- `GLFundamentals` implements the necessary OpenGL functionality including matrix mathematics and shader handling.

- `OVR_SDL2_app` implements a cross-platform VR application base class using SDL2 and the Oculus SDK.

- `OVR_SDL2_nav` subclasses `OVR_SDL2_app` to implement first-person-style navigation supporting both gamepad and keyboard-mouse inputs.

- `OVR_SDL2_obj` subclasses `OVR_SDL2_nav` to implement a Wavefront OBJ viewer application.

## Building

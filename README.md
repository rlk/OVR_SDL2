# Oculus SDL2 Examples

This is a set of minimal cross-platform example applications for the Oculus Rift written in C++ and depending only on SDL2 and the Oculus VR SDK version 0.4. These examples share a simple SDL2 application framework that provides an OpenGL 3.2 Core Profile and an FPS-style interaction. Their modular design enables easy reuse and its license permits you to copy it for any purpose.

## Structure

- `OVR_SDL2_app` implements a cross-platform VR application base class using SDL2 and the Oculus SDK.

	- `OVR_SDL2_nav` subclasses `OVR_SDL2_app` to implement first-person-style navigation supporting both gamepad and keyboard-mouse inputs.

		- `OVR_SDL2_room` subclasses `OVR_SDL2_nav` to implement a tiny room demo.

		- `OVR_SDL2_obj` subclasses `OVR_SDL2_nav` to implement a Wavefront OBJ viewer application.

- `GLFundamentals` implements the necessary OpenGL functionality including matrix mathematics and shader handling.

- `GLFramebuffer` encapsulates an OpenGL framebuffer object.

## Building

### Under OSX

1. Install the [SDL2](http://libsdl.org/download-2.0.php) framework to `/Library/Frameworks`.

2. Check the `Makefile` to ensure that the following definition gives the correct location for the Oculus SDK.

		OVR= /usr/local/OculusSDK/LibOVR

3. Build using make

		$ make


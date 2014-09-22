OPT= -g -Wall

# Configure for OS X

ifeq ($(shell uname), Darwin)
	CXX= c++ -F/Library/Frameworks
	LIB= -framework SDL2 -framework OpenGL
endif

# Configure for Linux

ifeq ($(shell uname), Linux)
	CXX= g++
	LIB= -lSDL2 -lGLEW -lGL -lm
endif

# Link the executables

all : OVR_SDL2_obj OVR_SDL2_room

OVR_SDL2_obj  : OVR_SDL2_app.o OVR_SDL2_nav.o OVR_SDL2_obj.o
	$(CXX) $(OPT) -o $@ $^ $(LIB)

OVR_SDL2_room : OVR_SDL2_app.o OVR_SDL2_nav.o OVR_SDL2_room.o
	$(CXX) $(OPT) -o $@ $^ $(LIB)

# Compile a module

.cpp.o :
	$(CXX) $(OPT) -c $<

# Clean up all binaries

clean :
	$(RM) OVR_SDL2_obj OVR_SDL2_room *.o

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

# Link the executable

OVR_SDL2_obj : OVR_SDL2_app.o OVR_SDL2_nav.o OVR_SDL2_obj.o main.o
	$(CXX) $(OPT) -o $@ $^ $(LIB)

# Compile a module

.cpp.o :
	$(CXX) $(OPT) -c $<

# Clean up all binaries

clean :
	$(RM) OVR_SDL2_obj *.o

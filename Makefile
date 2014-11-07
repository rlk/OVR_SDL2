OPT= -g -Wall
OVR= /usr/local/OculusSDK/LibOVR

# Configure for OS X

ifeq ($(shell uname), Darwin)
	CXX= c++ -std=c++11 -F/Library/Frameworks
	INC= -I$(OVR)/Include -I$(OVR)/Src
	LIB= -L$(OVR)/Lib/Mac/Release -lovr -framework SDL2 \
	                                    -framework IOKit \
	                                    -framework Foundation \
	                                    -framework CoreFoundation \
	                                    -framework CoreGraphics \
	                                    -framework OpenGL
endif

# Configure for Linux

ifeq ($(shell uname), Linux)
	CXX= g++ -std=c++11
	INC= -I$(OVR)/Include -I$(OVR)/Src
	LIB= -L$(OVR)/Lib/Linux/Release/x86_64 -lovr -lSDL2 -lGLEW -lGL -lX11 -lXrandr -lm -lpthread -lrt
endif

# Link the executables

all : OVR_SDL2_obj OVR_SDL2_room

OVR_SDL2_obj  : OVR_SDL2_app.o OVR_SDL2_nav.o OVR_SDL2_obj.o obj.o
	$(CXX) $(OPT) -o $@ $^ $(LIB)

OVR_SDL2_room : OVR_SDL2_app.o OVR_SDL2_nav.o OVR_SDL2_room.o
	$(CXX) $(OPT) -o $@ $^ $(LIB)

# Compile a module

.cpp.o :
	$(CXX) $(OPT) $(INC) -c $<

.c.o :
	$(CC) $(OPT) $(INC) -c $<

# Clean up all binaries

clean :
	$(RM) OVR_SDL2_obj OVR_SDL2_room *.o

# Define module dependencies

OVR_SDL2_app.o  : OVR_SDL2_app.hpp
OVR_SDL2_nav.o  : OVR_SDL2_app.hpp OVR_SDL2_nav.hpp
OVR_SDL2_obj.o  : OVR_SDL2_app.hpp OVR_SDL2_nav.hpp OVR_SDL2_obj.hpp
OVR_SDL2_room.o : OVR_SDL2_app.hpp OVR_SDL2_nav.hpp OVR_SDL2_room.hpp

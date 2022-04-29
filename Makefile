# g++ src/test.cpp src/glad.c -Iinclude `pkg-config --cflags --libs sdl2` -o test -ldl
CPPFLAGS += -Iinclude/
CFLAGS += -std=gnu11 -Wall -Wextra
CXXFLAGS += -std=gnu++14 -Wall -Wextra
LDFLAGS += -lSDL2 -ldl

obj/setupOGL: obj/glad.o obj/setup_ogl.o Makefile
	g++ obj/glad.o obj/setup_ogl.o $(CXXFLAGS) $(LDFLAGS) -o obj/setupOGL

obj/glad.o: src/glad.c include/glad/glad.h Makefile
	gcc src/glad.c $(CPPFLAGS) $(CFLAGS) -o obj/glad.o  -c

obj/setup_ogl.o: src/setup_ogl.cpp include/glad/glad.h Makefile
	g++ src/setup_ogl.cpp $(CPPFLAGS) $(CXXFLAGS) -o obj/setup_ogl.o -c

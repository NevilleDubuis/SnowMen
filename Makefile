#Compiler
CC = gcc

#Debug or Release
DEBUG = -g
RELEASE = -O2
EXEC_TYPE = ${RELEASE}

CFLAGS = -pipe -W -Wall -Wextra -Wunused-variable -pedantic-errors -std=c99 ${EXEC_TYPE}


OS := $(shell uname)
ifeq ($(OS),Darwin)
	#PATH
	PATH_X = -lXrandr -lXxf86vm -lXi
	PATH_OPENGL =
	INCPATH = ${PATH_X} ${PATH_OPENGL}

	#FLAGS
	FLAGS_X =
	FLAGS_OPENGL = -I/usr/X11/include/X11/extensions -L/usr/X11/lib
	OTHER_FLAGS = -framework OpenGL -framework GLUT
	LDFLAGS = ${FLAGS_X} ${FLAGS_OPENGL} ${OTHER_FLAGS}
else
	#PATH
	PATH_X =
	PATH_OPENGL =
	INCPATH = ${PATH_X} ${PATH_OPENGL}

	#FLAGS
	FLAGS_X =
	FLAGS_OPENGL = -lglut -lGLU -lGL
	OTHER_FLAGS = -lm
	LDFLAGS = ${FLAGS_X} ${FLAGS_OPENGL} ${OTHER_FLAGS}
endif

#compile every source files
all: bin/main.o bin/render.o
	@echo --------- build [$<] ----------
	$(CC) $^ -o game $(LDFLAGS) $(INCPATH)
	@echo OK [$<] - [$@]
	@echo

bin/main.o: src/render.h

bin/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCPATH)

#make clean
clean:
	rm -rf bin/*.o ./Glut

#
# Help varaible interne
#
# $@ : nom de la cible
# $< : nom de la premiere dependance
# $^ : liste des dépendances
# $? : dépendance plus récentes que la cible
# $* : nom du fichier sans le suffixe
#
#

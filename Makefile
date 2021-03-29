SHELL=cmd
ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

MKDIR   := mkdir
TEST    := test
RMDIR   := rmdir /Q /S
CC      := gcc
OBJ_INIT:= init
INCLUDE := include
SRCS    := $(wildcard *.c)
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXE     := combat.exe
CFLAGS  := -g -I
LDLIBS  := -lm

.PHONY: all run clean path

all: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $^ -o $@ $(PATH_ALLEGRO)$(LIB_ALLEGRO)

%.o: %.c
	if not exist "$(@D)" $(MKDIR) $(subst /,\\,$(@D))
	$(CC) $(CFLAGS) $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c $< -o $@


run: $(EXE)
	$<

clean:
	del *.o
	del *.exe
# OCR_Gang's Makefile
#Author : marius.andre

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= main.c source/process/process.c source/sdl/our_sdl.c source/segmentation/segmentation.c source/nn/nn.c source/nn/tools.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main create

create:
	[ -d "source/Xor" ] || mkdir source/Xor
	[ -d "img/temp" ] || mkdir img/temp
	touch source/Xor/Xor-data.txt
	touch source/Xor/Xor-weights.txt

main: $(OBJ)

clean:
	rm -f *.bmp ; rm -f img/temp/*.bmp ; rm -f source/Xor/Xor-*
	$(RM) $(OBJ) $(OBJ_TESTS) $(DEP) $(DEP_TESTS) main && clear
# END

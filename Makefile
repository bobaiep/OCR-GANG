# OCR_Gang's Makefile
#Author : marius.andre

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= main.c source/process/process.c source/sdl/our_sdl.c source/segmentation/segmentation.c source/network/network.c source/network/tools.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main create

create:
	[ -d "source/Xor" ] || mkdir source/Xor
	[ -d "img/temp" ] || mkdir img/temp
	[ -d "source/OCR" ] || mkdir source/OCR
	touch source/Xor/xordata.txt
	touch source/Xor/xorwb.txt
	touch source/OCR/ocrdata.txt
	touch source/OCR/ocrwb.txt

main: $(OBJ)

clean:
	rm -rf *.bmp img/temp/*.bmp source/Xor source/OCR *.tst
	$(RM) $(OBJ) $(OBJ_TESTS) $(DEP) $(DEP_TESTS) main && clear
# END

TARGET= AirGuitarHero
SOURCES= accelerometer.c buttonArray.c displayModel.c gameLogicHandler.c gameStart.c noteQueue.c songList.c utils.c wave_player.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

#CFLAGS = -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -Wall
CFLAGS = -g -std=c99 -D _POSIX_C_SOURCE=200809L -Wshadow -Wall

# Asound Library
LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: AirGuitarHero cpy

AirGuitarHero:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) $(LFLAGS) -lpthread -lasound

cpy:
	cp *.txt $(OUTDIR)

clean:
	rm -f $(OUTDIR)/$(TARGET)
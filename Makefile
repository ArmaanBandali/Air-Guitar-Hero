CC_C = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -pthread

OBJFILES = accelerometer.c main.c 
OUTFILE = main

OUTDIR = $(HOME)/cmpt433/public/myApps/

all: $(OUTFILE)

$(OUTFILE): $(OBJFILES)
	$(CC_C) $(CFLAGS) $(OBJFILES) -o $(OUTFILE) 
	cp SampleNotes.txt $(OUTDIR)
	cp $(OUTFILE) $(OUTDIR)

clean:
	rm -rf $(OUTFILE) $(OUTFILE) *~

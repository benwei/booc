CFILES = parse.c
OBJS = $(CFILES:.c=.o)
INCS =-I.
CFLAGS = -Wall -std=c99 -pedantic 
#CFLAGS = -Wall  -g
CFLAGS += $(INCS)
LDFLAGS =

TARGET = ooc_calc

all: $(TARGET);

tests: $(TARGET)
	sh ./mytests.sh $(TARGET)

include ./mk.rules

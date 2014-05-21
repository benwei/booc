CFILES = parse.c bcalc.c
OBJS = $(CFILES:.c=.o)
INCS =-I.
CFLAGS = -Wall -std=c99 -pedantic 
#CFLAGS = -Wall  -g
CFLAGS += $(INCS)
LDFLAGS =

TARGET += bcalc
.PHONY: tests

all: $(TARGET);

tests: $(TARGET)
	sh ./tests.sh $(TARGET)

run: $(TARGET)
	cat tests/file01.txt | ./$(TARGET)
include ./mk.rules

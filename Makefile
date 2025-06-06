project = mem
outputfile = $(project)

OBJS = main.o $(project).o memset.o

CC = gcc
AS = as
CFLAGS := -Wall -I$(COA_ARM64)/include
LDLIBS := -L$(COA_ARM64)/lib -lgrlcoa

.PHONY: all clean

all: $(outputfile)

$(outputfile): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

main.o: main.c $(project).h
	$(CC) $(CFLAGS) -c $< -o $@

$(project).o: $(project).c $(project).h
	$(CC) $(CFLAGS) -c $< -o $@

memset.o: memset.S
	$(AS) memset.S -o $@

clean:
	rm -f *.o $(outputfile)

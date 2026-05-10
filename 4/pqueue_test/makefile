holdall_dir = ../holdall/
pqueue_dir = ../pqueue/
CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(holdall_dir) -I$(pqueue_dir)
vpath %.c $(holdall_dir) $(pqueue_dir)
vpath %.h $(holdall_dir) $(pqueue_dir)
objects = main.o holdall.o pqueue.o
executable = test
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c holdall.h holdall_ip.h pqueue.h pqueue_ip.h
holdall.o: holdall.c holdall.h holdall_ip.h
pqueue.o: pqueue.c pqueue.h pqueue_ip.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)

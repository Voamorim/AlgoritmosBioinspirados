COMPILER = gcc
FLAGS = -Wall -Wextra -g
BINARYDIR = binary
EXENAME = ag

SRCS = $(filter-out main.c, $(wildcard *.c))
OBJS = $(SRCS:%.c=$(BINARYDIR)/%.o)

.PHONY: all clean

all: $(EXENAME)

$(EXENAME): $(OBJS) $(BINARYDIR)/main.o
		$(COMPILER) $(FLAGS) $^ -o $@ -lm

$(BINARYDIR)/%.o: %.c %.h
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR)/main.o: main.c
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm


clean:
	rm -f $(BINARYDIR)/* $(EXENAME)

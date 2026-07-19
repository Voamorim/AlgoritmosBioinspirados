COMPILER = gcc
FLAGS = -Wall -Wextra -g
BINARYDIR = binary
EXENAME = ag_real

SRCS = $(filter-out main.c, $(wildcard *.c))
OBJS = $(SRCS:%.c=$(BINARYDIR)/%.o)

.PHONY: all clean

all: $(BINARYDIR) $(EXENAME)

$(EXENAME): $(OBJS) $(BINARYDIR)/main.o
		$(COMPILER) $(FLAGS) $^ -o $@ -lm

$(BINARYDIR)/%.o: %.c %.h
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR)/main.o: main.c
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR):
	test ! -d $(BINARYDIR) && mkdir $(BINARYDIR)

clean:
	rm -rf $(BINARYDIR)/* $(EXENAME)

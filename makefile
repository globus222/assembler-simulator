CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
LDFLAGS = -lm

SRCDIR = .
SOURCES = program.c types.h quantities.h data_structures/dynamic_array.c data_structures/dynamic_array.h \
    command_analysis/tokenizer.c command_analysis/reader.c command_analysis/command_analysis.h \
    command_analysis/macros_table.c function_macros.h absolutes.h command_analysis/symbols_table.c \
    error_detection/errors.h command_analysis/helpers.c command_analysis/iterations.c \
    command_analysis/commands_table.c command_analysis/helpers.h error_detection/detector.c \
    error_detection/detector.h segments.h definitions.c compiler.c error_detection/helpers.c \
    error_detection/helpers.h command_analysis/files.c compiler.h

OBJDIR = build
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean

all: assembler_simulator

assembler_simulator: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf assembler_simulator $(OBJDIR)

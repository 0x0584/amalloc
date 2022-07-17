LIBRARY      = amalloc
LIBRARY_NAME = $(LIBRARY).so

DEBUG  = on
BUILD  = off

SRCDIR = src
INCDIR = include
OBJDIR = .obj

SRCS := $(shell find $(SRCDIR) -type f -name '*.c')
INCS := $(shell find $(INCDIR) -type f -name '*.h')
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

CC     = gcc
LANGSTD = c11
CFLAGS = -std=$(LANGSTD) -Wall -Wextra -Wpedantic -shared

ifdef ($(DEBUG),on)
	CFLAGS += -Og -g
endif

ifdef ($(BUILD),on)
	CFLAGS += -O3 -NDEBUG
endif

all: $(OBJS)
	@printf ""
	@$(CC) $(CFLAGS) -o $(LIBRARY_NAME) $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCS)
	@echo "CC " $<
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

info:
	@printf "Compilation commands: ["
	@printf "make"
	@printf "make all"
	@printf "make info ]\n"

	@echo "CC = " $(CC)
	@echo "CFLAGS = " $(CFLAGS)

.PHONY: all info

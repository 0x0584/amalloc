
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)-$(shell uname -s)
endif

LIB=libft_malloc
LIB_DIR=$(shell pwd)
LIB_FILE=$(LIB_DIR)/$(LIB)_$(HOESTTYPE).so

FT=libft

SRCDIR=src
INCLUDE=include
OBJDIR=.objs

RM=rm -f

SRCS:=$(shell find $(SRCDIR) -type f -name '*.c')
HEADERS:=$(shell find $(INCLUDE) -type f -name '*.h')
OBJS:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

CC=clang
CFLAGS=-Wall -Wextra -I$(INCLUDE) -I$(FT)
CXX=clang++
CXXFLAGS=$(CFLAGS) -std=c++14
LDFLAGS=-shared -L$(FT) -l$(FT)

GTEST_DIR=googletest
GTEST_SRCS=$(GTEST_DIR)/src/gtest{,_main}.cc
GTEST_INCLUDE_FLAGS=-I$(GTEST_DIR)/include -I$(GTEST_DIR)/src
CXXFLAGS+=$(GTEST_INCLUDE_FLAGS)

TEST_DIR=tests
TEST_SRCS:=$(shell find $(TEST_DIR) -type f -name '*.cc')
TESTS:=$(patsubst $(TEST_DIR)/%.cc,$(TEST_DIR)/%.test,$(TEST_SRCS))

all: ft $(LIB)

ft:
	$(MAKE) -C $(FT)

$(LIB): ft $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(LIB_FILE) $(LDFLAGS)
#	setenv DYLD_LIBRARY_PATH $(LIB_DIR)
#   setenv DYLD_INSERT_LIBRARIES $(LIB_FILE)
#   setenv DYLD_FORCE_FLAT_NAMESPACE 1

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) ~/.objs/*
	$(RM) $(LIB_FILE)

test: $(TESTS)

$(TEST_DIR)/%.test: $(TEST_DIR)/%.cc $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(GTEST_SRCS) $(LIB_FILE) $< -o $@

re: clean all

.PHONY: ft re all clean test

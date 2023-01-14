
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)-$(shell uname -s)
endif

PWD?=$(shell pwd)

LIB=libft_malloc
LIB_DIR=$(PWD)
LIB_FILE=$(LIB_DIR)/$(LIB)_$(HOSTTYPE).so

SRCDIR=$(PWD)/src
INCLUDE=$(PWD)/include
OBJDIR=$(PWD)/bin

RM=rm -f

SRCS:=$(shell find $(SRCDIR) -type f -name '*.c')
HEADERS:=$(shell find $(INCLUDE) -type f -name '*.h')
OBJS:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

CC=clang
CFLAGS=-Wall -Wextra -I$(INCLUDE) -I$(FT) -fPIC

GTEST_DIR=googletest
GTEST_SRCS=$(GTEST_DIR)/src/gtest{,_main}.cc
GTEST_INCLUDE_FLAGS=-I$(GTEST_DIR)/include -I$(GTEST_DIR)/src

CXX=clang++
CXXFLAGS=$(GTEST_INCLUDE_FLAGS) $(CFLAGS) -std=c++14

TEST_DIR=tests
TEST_SRCS:=$(shell find $(TEST_DIR) -type f -name '*.cc')
TESTS:=$(patsubst $(TEST_DIR)/%.cc,$(TEST_DIR)/%.test,$(TEST_SRCS))

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) -shared $(OBJS) -o $(LIB_FILE)

#	setenv DYLD_LIBRARY_PATH $(LIB_DIR)
#   setenv DYLD_INSERT_LIBRARIES $(LIB_FILE)
#   setenv DYLD_FORCE_FLAT_NAMESPACE 1

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
#	$(MAKE) -C $(FT) clean
	$(RM) $(OBJDIR)/*
	$(RM) $(LIB_FILE)

test: $(TESTS)

$(TEST_DIR)/%.test: $(TEST_DIR)/%.cc $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(GTEST_SRCS) $(LIB_FILE) $< -o $@

re: clean all

.PHONY: ft re all clean test

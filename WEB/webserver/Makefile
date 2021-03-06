# Makefile for httpserver
# (C) Ramsey Kant 2011-2012

CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGET := httpserver
UNAME := $(shell uname)

# Debug Flags
DEBUGFLAGS := -g3 -O0 -Wall
RTCHECKS := -fmudflap -fstack-check -gnato

# Production Flags
PRODFLAGS := -Wall -O2

ifeq ($(UNAME), Linux)
# Linux Flags
CFLAGS := -std=c++11 -Iinclude/ $(DEBUGFLAGS)
LINK := -lpthread -lkqueue $(DEBUGFLAGS)
#LINK := -lpthread -lkqueue $(DEBUGFLAGS)
else
# OSX / BSD Flags
CFLAGS := -std=c++11 -stdlib=libc++ -Iinclude/ $(DEBUGFLAGS)
LINK := -stdlib=libc++ $(DEBUGFLAGS)
endif
 
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@echo " Linking..."$(LINK); $(CC) $^ -o $(BINDIR)/$(TARGET) $(LINK)
 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<
 
clean:
	@echo " Cleaning..."; rm -r $(BUILDDIR) $(BINDIR)/$(TARGET)*
 
.PHONY: clean

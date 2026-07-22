.PHONY: default clean

CC ?= cc
RM ?= rm -f

CPPFLAGS += $(shell pkg-config --cflags sdl3)
CFLAGS += -g -O2 -Wall -Wpedantic -Wextra -MMD -MP
LDLIBS += -lm $(shell pkg-config --libs sdl3) -mconsole

TARGET_NAME ?= InputLogger
ifeq ($(OS),Windows_NT)
TARGET ?= $(TARGET_NAME).exe
else
TARGET ?= $(TARGET)
endif

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS) $(OBJS:.o=.d)

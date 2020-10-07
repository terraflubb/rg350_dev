CC        := /root/buildroot/output/host/bin/mipsel-rg350-linux-uclibc-g++
STRIP     := /root/buildroot/output/host/bin/mipsel-rg350-linux-uclibc-strip
LIBS      := -L/root/buildroot/output/host/mipsel-rg350-linux-uclibc/sysroot/usr/lib
INCS      := -I/root/buildroot/output/host/mipsel-rg350-linux-uclibc/sysroot/usr/include
TARGET    := program.gcw
SYSROOT   := $(shell $(CC) --print-sysroot)
CFLAGS    := $(LIBS) -lz -lm -ldl -lGLESv2 -lEGL -lSDL2
SRCDIR    := src
OBJDIR    := obj
SRC       := $(wildcard $(SRCDIR)/*.c)
OBJ       := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
SOVERSION := $(TARGET).0

ifdef DEBUG
  CFLAGS += -ggdb -Wall -Werror
else
  CFLAGS += -O2
endif

.PHONY: all clean opk

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)
ifdef DO_STRIP
	$(STRIP) $@
endif

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $< -o $@ $(INCS) -DPLATFORM_LINUX

$(OBJDIR):
	mkdir -p $@

clean:
	rm -Rf $(TARGET) $(OBJDIR)
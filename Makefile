TARGET = robotd
OBJECT = robotd.o
SOURCE = robotd.c
CFLAGS = -std=c11 -Wall -g -O0 `pkg-config --cflags python`
LDLIBS = -lmraa `pkg-config --libs python`

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECT)

$(OBJECT): $(SOURCE)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECT)

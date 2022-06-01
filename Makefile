CFLAGS = -Wall -Wextra
ifdef DEBUG
CFLAGS := $(CFLAGS) -ggdb
endif

OBJECTS := gpio.o

.PHONY: all
all: gpio_fan

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

gpio_fan: gpio_fan.c $(OBJECTS)
	$(CC) gpio_fan.c $(OBJECTS) -o gpio_fan $(CFLAGS)

.PHONY: clean
clean:
	$(RM) gpio_fan $(OBJECTS)

.PHONY: install
install: all
	install -m755 gpio_fan /usr/bin/
	install -m644 gpio_fan.service /usr/lib/systemd/system/

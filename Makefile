.PHONY: default all clean
SRCDIR=src
TARGET=kodilauncher
CC=gcc
CFLAGS=-Wall

default: build
all: default

$(SRCDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build: $(SRCDIR)/$(TARGET).o
	$(CC) $< -o $(TARGET)

clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	cp res/$(TARGET).service /usr/lib/systemd/system/
	systemctl enable $(TARGET).service 
	systemctl daemon-reload
	systemctl restart $(TARGET).service

uninstall:
	systemctl stop $(TARGET).service 
	systemctl disable $(TARGET).service 
	rm -f /usr/local/bin/$(TARGET)
	rm -f /usr/lib/systemd/system/$(TARGET).service
	systemctl daemon-reload

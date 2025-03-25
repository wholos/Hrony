C=gcc
CFLAGS=-o repkg -g -Wall -I/usr/include -lcurl -ljson-c

all:
	@echo "Build..."
	@$(C) $(CFLAGS) src/main.c

clean:
	@echo "Clean..."
	@rm repkg

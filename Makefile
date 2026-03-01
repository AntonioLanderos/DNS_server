# Default target. Running 'make' builds everything. If you build multiple binaries, they
# all get listed here.
all: build/udpserver

# Build the udp echo server specifically. We create the build dir if it's not already
# there (-p flag) and then tell the C compiler to write to that dir. Everything in
# build/* is in the .gitignore, so untracked
build/udpserver: udpserver.c
	mkdir -p build
	$(CC) udpserver.c -o build/udpserver

# Try to look up google.com
test: build/udpserver
	dig google.com @127.0.0.1 -p 7777

# Blow everything away in one go
clean:
	rm -rf build

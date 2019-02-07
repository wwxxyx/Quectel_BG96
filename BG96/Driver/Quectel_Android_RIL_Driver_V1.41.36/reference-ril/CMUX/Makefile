CROSS-COMPILE:=
CC:=$(CROSS-COMPILE)gcc
LD:=$(CROSS-COMPILE)ld

release: clean
	$(CC) -Wall -s gsm0710muxd_bp.c -o gsm0710muxd_bp -lpthread

debug: clean
	$(CC) -Wall -g gsm0710muxd_bp.c -o gsm0710muxd_bp -lpthread

clean:
	rm -rf gsm0710muxd_bp

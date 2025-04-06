clock: clock.c
	gcc -Wall -O3 -fdata-sections -ffunction-sections -L/usr/local/lib -I/usr/local/include clock.c -lraylib -lm -o clock -Wl,--gc-sections

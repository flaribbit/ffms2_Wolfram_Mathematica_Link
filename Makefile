CC=gcc
CFLAGS=ffms2.lib -O2 -s -DNDEBUG
WOLFRAM=WolframRTL.lib

ffmslink.dll: main.c
	$(CC) main.c $(CFLAGS) $(WOLFRAM) -shared -o ffmslink.dll

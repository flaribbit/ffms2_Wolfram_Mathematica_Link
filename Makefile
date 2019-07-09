CC=gcc
CFLAGS=ffms2.lib -O2 -s -DNDEBUG
WOLFRAM=WolframRTL.lib

main.exe: main.c
	$(CC) main.c $(CFLAGS) $(WOLFRAM) -o main.exe

main.dll: main.c
	$(CC) main.c $(CFLAGS) $(WOLFRAM) -shared -o main.dll

demo.dll: demo.c
	$(CC) demo.c $(CFLAGS) $(WOLFRAM) -shared -o demo.dll

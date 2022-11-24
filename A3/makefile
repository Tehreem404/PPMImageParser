CFLAGS = -ansi -I/usr/include/python3.8 -I/usr/include/python3.8  -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.8-YxOiQL/python3.8-3.8.10=. -specs=/usr/share/dpkg/no-pie-compile.specs -fstack-protector -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall -fPIC -std=c99
LDFLAGS = -L/usr/lib/python3.8/config-3.8-x86_64-linux-gnu -L/usr/lib -lpython3.8 -lcrypt -lpthread -ldl  -lutil -lm -lm
CC = gcc

all: a1.o functions.o
	$(CC) a1.o functions.o -o a1 $(LDFLAGS)

a1.o: a1.c functions.h
	$(CC) a1.c -o a1.o -c $(CFLAGS)

functions.o: functions.c functions.h
	$(CC) functions.c -o functions.o -c $(CFLAGS)

clean:
	rm *.o -i a1
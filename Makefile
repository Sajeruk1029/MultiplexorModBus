CC	=	gcc

SOURCES	=	main.c server.c conf.c
OBJECTS	=	main.o server.o conf.o

WFLAGS	=	-Wall -Wextra -Wpedantic
OFLAGS	=	-O2 -flto -fuse-linker-plugin
LFLAGS	=	-lmodbus -Wl,--gc-sections
PFLAGS	=	-march=native
DFLAGS	=	-g

LIBS	=	-L /usr/lib/libmodbus.so
HEADERSLIBS	=	-I /usr/include/modbus/

PROGNAME	=	Server

RM	= rm -rf

all	:	$(PROGNAME)

$(PROGNAME)	:	$(OBJECTS)
	$(CC) $(WFLAGS) $(OFLAGS) $(OBJECTS) -o $(PROGNAME) $(LIBS) $(LFLAGS) $(HEADERSLIBS) $(DFLAGS) $(PFLAGS)

main.o	:	main.c
	$(CC) $(WFLAGS) $(OFLAGS) main.c -c $(LIBS) $(LFLAGS) $(HEADERSLIBS) $(MACROSES) $(DFLAGS) $(PFLAGS)

server.o	:	server.c
	$(CC) $(WFLAGS) $(OFLAGS) server.c -c $(LIBS) $(LFLAGS) $(HEADERSLIBS) $(DFLAGS) $(PFLAGS)

conf.o	:	conf.c
	$(CC)	$(WFLAGS) $(OFLAGS) conf.c -c $(LIBS) $(LFLAGS) $(HEADERSLIBS) $(DFLAGS) $(PFLAGS)

clean	:
	$(RM)	$(OBJECTS) $(PROGNAME)

install	:
	install $(PROGNAME) /bin/

uninstall	:
	rm -rf /bin/$(PROGNAME)

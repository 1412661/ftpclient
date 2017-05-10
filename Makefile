CC = gcc
LIBS =

# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# Can use #pragma GCC diagnostic ignored in code
# #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
# #pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

IGNORE = -Wno-incompatible-pointer-types -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
ARGS = -std=gnu99

all:
	$(CC) $(ARGS) -c function.c $(LIBS)
	$(CC) $(ARGS) -c ftp.c $(LIBS)
	$(CC) $(IGNORE) $(ARGS) -o ftp ftp.o function.o $(LIBS)
	@rm *.o

Release:
	$(CC) $(ARGS) -c function.c $(LIBS)
	$(CC) $(ARGS) -c ftp.c $(LIBS)
	$(CC) $(IGNORE) $(ARGS) -o ftp ftp.o function.o $(LIBS)
	@rm *.o

Debug:
	$(CC) $(ARGS) -g -c function.c $(LIBS)
	$(CC) $(ARGS) -g -c ftp.c $(LIBS)
	$(CC) $(IGNORE) $(ARGS) -g -o ftp ftp.o function.o $(LIBS)
	@rm *.o

cleanDebug:
	clean.sh || :

cleanRelease:
	clean.sh || :

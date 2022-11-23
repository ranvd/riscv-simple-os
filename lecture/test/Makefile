FILE = main.c \
	lib.c \
	user.c
OBJ_FILE = $(FILE:.c=.o)

all: a.out
	

a.out: $(OBJ_FILE)
	gcc -o $@ $^

%.o: %.c
	gcc -c $^

.PHONY=clean
clean :
	rm -rf *.o a.out
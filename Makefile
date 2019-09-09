all: main
.PHONY: all

clean:
	rm -f *.o main

main: main.o imge.o quad.o
	gcc -g $^ -o main -lm

main.o : imge.h quad.h

imge.o : imge.h

quad.o : quad.h imge.h

%.o : %.c
	gcc -g -c $<

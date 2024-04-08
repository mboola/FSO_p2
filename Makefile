FLAGS_TENNIS	=	-Wall -Wextra -Werror
INCLUDE			=	-I ./
OPTIMIZATION	=	-03

#versions
TENNIS0_SRC		=	./tennis0_files/
FILES0			=	${TENNIS0_SRC}ball.c ${TENNIS0_SRC}computer.c \
					${TENNIS0_SRC}tennis0.c ${TENNIS0_SRC}user.c

all : winsuport.o tennis0 tennis1 tennis2

winsuport.o : winsuport.c winsuport.h
	gcc -Wall -c winsuport.c -o winsuport.o

tennis0 : winsuport.o winsuport.h
	gcc ${FLAGS_TENNIS} ${FILES0} winsuport.o -o tennis0 -lcurses ${INCLUDE}

tennis1 : tennis1.c winsuport.o winsuport.h
	gcc -Wall tennis1.c winsuport.o -o tennis1 -lcurses -lpthread

tennis2 : tennis2.c winsuport.o winsuport.h
	gcc tennis2.c winsuport.o -o tennis2 -lcurses -lpthread

clean: 
	rm winsuport.o tennis0 tennis1 tennis2

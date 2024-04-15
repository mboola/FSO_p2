####### COMPILATION FLAGS #######
FLAGS_TENNIS	=	-Wall -Wextra -Werror
INCLUDE			=	-I ./
OPTIMIZATION	=	-03

FILES_COMP	=	${FILES} ${TENNIS_SRC}${TENNIS_MAIN}

####### FILES #######
FILES	=	${TENNIS_SRC}ball.c ${TENNIS_SRC}computer.c ${TENNIS_SRC}user.c \
			${TENNIS_SRC}timer.c
HEADER	=	${TENNIS_SRC}tennis.h

EXECS	=	tennis0_exe tennis1_exe tennis2_exe

####### RULES #######
all: winsuport.o tennis0 tennis1 tennis2

winsuport.o: winsuport.c winsuport.h
	gcc -Wall -c winsuport.c -o winsuport.o

tennis0: TENNIS_SRC := ./tennis0_files/
tennis0: TENNIS_EXE := tennis0_exe
tennis0: TENNIS_MAIN := tennis0.c

tennis1: TENNIS_SRC := ./tennis1_files/
tennis1: TENNIS_EXE := tennis1_exe
tennis1: TENNIS_MAIN := tennis1.c

tennis2: TENNIS_SRC := ./tennis2_files/
tennis2: TENNIS_EXE := tennis2_exe
tennis2: TENNIS_MAIN := tennis2.c

tennis0 tennis1 tennis2: winsuport.o winsuport.h
	gcc ${FLAGS_TENNIS} ${FILES_COMP} winsuport.o -o ${TENNIS_EXE} -lcurses ${INCLUDE} -I ${TENNIS_SRC}

clean:
	rm winsuport.o
	rm ${EXECS}

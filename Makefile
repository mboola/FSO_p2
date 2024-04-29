####### COMPILATION FLAGS #######
FLAGS_TENNIS	=	-Wall -Wextra -Werror
FLAGS_LIBS		=	-Wall
INCLUDE			=	-I ./
OPTIMIZATION	=	-03

FILES_COMP	=	${FILES} ${TENNIS_SRC}${TENNIS_MAIN} ${TENNIS_}

####### FILES #######
FILES	=	${TENNIS_SRC}ball.c ${TENNIS_SRC}computer.c ${TENNIS_SRC}user.c \
			${TENNIS_SRC}timer.c
HEADER	=	${TENNIS_SRC}tennis.h

EXECS	=	tennis0_exe tennis1_exe tennis2_exe tennis3_exe tennis4_exe

THREAD_HEADER	=	winsuport.h
THREAD_FILES	=	winsuport.c
THREAD_OBJECTS	=	${THREAD_FILES:.c=.o}
PROCESS_HEADER	=	winsuport2.h semafor.h memoria.h missatge.h
PROCESS_FILES	=	winsuport2.c semafor.c memoria.c missatge.c
PROCESS_OBJECTS	=	${PROCESS_FILES:.c=.o}

%.o: %.c
	gcc ${FLAGS_LIBS} -c $< -o $@

####### RULES #######
all: ${THREAD_OBJECTS} ${PROCESS_OBJECTS} tennis0 tennis1 tennis2 tennis3 tennis4

tennis0: TENNIS_SRC := ./tennis0_files/
tennis0: TENNIS_EXE := tennis0_exe
tennis0: TENNIS_MAIN := tennis0.c

tennis1: TENNIS_SRC := ./tennis1_files/
tennis1: TENNIS_EXE := tennis1_exe
tennis1: TENNIS_MAIN := tennis1.c

tennis2: TENNIS_SRC := ./tennis2_files/
tennis2: TENNIS_EXE := tennis2_exe
tennis2: TENNIS_MAIN := tennis2.c

tennis3: TENNIS_SRC := ./tennis3_files/
tennis3: TENNIS_EXE := tennis0_exe
tennis3: TENNIS_MAIN := tennis3.c

tennis4: TENNIS_SRC := ./tennis4_files/
tennis4: TENNIS_EXE := tennis0_exe
tennis4: TENNIS_MAIN := tennis4.c

tennis0 tennis1 tennis2: ${THREAD_OBJECTS} ${THREAD_HEADER}
	gcc ${FLAGS_TENNIS} ${FILES_COMP} ${THREAD_OBJECTS} -o ${TENNIS_EXE} -lcurses ${INCLUDE} -I ${TENNIS_SRC}

tennis3 tennis4: ${PROCESS_OBJECTS} ${PROCESS_HEADER}
	gcc	${FLAGS_TENNIS} ${FILES_COMP} ${PROCESS_OBJECTS} -o ${TENNIS_EXE} -lcurses ${INCLUDE} -I ${TENNIS_SRC}

clean:
	rm ${THREAD_OBJECTS}
	rm ${PROCESS_OBJECTS}
	rm ${EXECS}

re: clean all

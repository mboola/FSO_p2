####### DIRS #######
LIB_DIR	=	./libs/

####### COMPILATION FLAGS #######
FLAGS_EXE	=	-Wall -Wextra -Werror
FLAGS_LIBS		=	-Wall
INCLUDE			=	-I ${LIB_DIR}
OPTIMIZATION	=	-03

####### FILES #######
FILES	=	${TENNIS_SRC}ball.c ${TENNIS_SRC}user.c ${TENNIS_SRC}timer.c ${TENNIS_SRC}error_handling.c

FILES_TENNIS = ${FILES} ${TENNIS_MAIN}

HEADER	=	${TENNIS_SRC}tennis.h

EXECS	=	tennis0 tennis1 tennis2 tennis3 tennis4

THREAD_HEADER	=	${LIB_DIR}winsuport.h
THREAD_FILES	=	${LIB_DIR}winsuport.c
THREAD_OBJECTS	=	${THREAD_FILES:.c=.o}

PROCESS_HEADER	=	${LIB_DIR}winsuport2.h ${LIB_DIR}semafor.h ${LIB_DIR}memoria.h ${LIB_DIR}missatge.h
PROCESS_FILES	=	${LIB_DIR}winsuport2.c ${LIB_DIR}semafor.c ${LIB_DIR}memoria.c ${LIB_DIR}missatge.c
PROCESS_OBJECTS	=	${PROCESS_FILES:.c=.o}

%.o: %.c
	gcc ${FLAGS_LIBS} -c $< -o $@


####### RULES #######
all: tennis0 tennis1 tennis2 tennis3 #tennis4

tennis0: TENNIS_SRC := ./tennis0_files/
tennis0: TENNIS_EXE := tennis0
tennis0: TENNIS_MAIN := ${TENNIS_SRC}tennis0.c

tennis1: TENNIS_SRC := ./tennis1_files/
tennis1: TENNIS_EXE := tennis1
tennis1: TENNIS_MAIN := ${TENNIS_SRC}tennis1.c

tennis2: TENNIS_SRC := ./tennis2_files/
tennis2: TENNIS_EXE := tennis2
tennis2: TENNIS_MAIN := ${TENNIS_SRC}tennis2.c

tennis0 tennis1 tennis2: ${THREAD_OBJECTS} ${THREAD_HEADER}
	gcc ${FLAGS_EXE} ${FILES_TENNIS} ${TENNIS_SRC}computer.c ${THREAD_OBJECTS} -o ${TENNIS_EXE} -lcurses ${INCLUDE}

tennis3: TENNIS_SRC := ./tennis3_files/
tennis3: TENNIS_EXE := tennis3
tennis3: TENNIS_EXE_CHILD := tennis3_ord
tennis3: TENNIS_MAIN := ${TENNIS_SRC}tennis3.c
tennis3: TENNIS_MAIN_CHILD := ${TENNIS_SRC}pal_ord3.c

#tennis4: TENNIS_SRC := ./tennis4_files/
#tennis4: TENNIS_EXE := tennis0_exe
#tennis4: TENNIS_MAIN := tennis4.c

tennis3 tennis4: ${PROCESS_OBJECTS} ${PROCESS_HEADER}
	gcc ${FLAGS_EXE} ${FILES_TENNIS} ${PROCESS_OBJECTS} -o ${TENNIS_EXE} -lcurses ${INCLUDE}
	gcc ${FLAGS_EXE} ${TENNIS_MAIN_CHILD} ${PROCESS_OBJECTS} -o ${TENNIS_EXE_CHILD} -lcurses ${INCLUDE}

clean:
	rm ${THREAD_OBJECTS}
	rm ${PROCESS_OBJECTS}
	rm ${EXECS}

re: clean all

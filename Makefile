####### DIRS #######
LIB_DIR	=	./libs/

####### COMPILATION FLAGS #######
FLAGS_EXE		=	-Wall -Wextra -Werror
FLAGS_LIBS		=	-Wall
INCLUDE			=	-I ${LIB_DIR}
OPTIMIZATION	=	-03

####### FILES #######
TENNIS0_SRC		=	./tennis0_files/
TENNIS0_FILES	=	${TENNIS0_SRC}ball.c ${TENNIS0_SRC}user.c \
					${TENNIS0_SRC}computer.c ${TENNIS0_SRC}tennis0.c
TENNIS0_EXE		=	tennis0

TENNIS1_SRC		=	./tennis1_files/
TENNIS1_FILES	=	${TENNIS1_SRC}ball.c ${TENNIS1_SRC}user.c ${TENNIS1_SRC}timer.c \
					${TENNIS1_SRC}computer.c ${TENNIS1_SRC}tennis1.c
TENNIS1_EXE		=	tennis1

TENNIS2_SRC		=	./tennis2_files/
TENNIS2_FILES	=	${TENNIS2_SRC}ball.c ${TENNIS2_SRC}user.c ${TENNIS2_SRC}timer.c \
					${TENNIS2_SRC}computer.c ${TENNIS2_SRC}tennis2.c ${TENNIS2_SRC}error_handling.c
TENNIS2_EXE		=	tennis2

TENNIS3_SRC		=	./tennis3_files/
TENNIS3_FILES	=	${TENNIS3_SRC}ball.c ${TENNIS3_SRC}user.c ${TENNIS3_SRC}timer.c \
					${TENNIS3_SRC}error_handling.c ${TENNIS3_SRC}tennis3.c
TENNIS3_PROC_FILES	=	${TENNIS3_SRC}pal_ord3.c
TENNIS3_EXE			=	tennis3
TENNIS3_PROC_EXE	=	pal_ord3

TENNIS4_SRC		=	./tennis4_files/
TENNIS4_FILES	=	${TENNIS4_SRC}ball.c ${TENNIS4_SRC}user.c ${TENNIS4_SRC}timer.c \
					${TENNIS4_SRC}error_handling.c ${TENNIS4_SRC}tennis4.c ${TENNIS4_SRC}tennis4_utils.c
TENNIS4_PROC_FILES	=	${TENNIS4_SRC}pal_ord4.c
TENNIS4_EXE			=	tennis4
TENNIS4_PROC_EXE	=	pal_ord4

EXECS	=	${TENNIS0_EXE} ${TENNIS1_EXE} ${TENNIS2_EXE} ${TENNIS3_EXE} ${TENNIS3_PROC_EXE} ${TENNIS4_EXE} ${TENNIS4_PROC_EXE}

####### LIBS FUNCTIONS #######
THREAD_HEADER	=	${LIB_DIR}winsuport.h
THREAD_FILES	=	${LIB_DIR}winsuport.c
THREAD_OBJECTS	=	${THREAD_FILES:.c=.o}

PROCESS_HEADER	=	${LIB_DIR}winsuport2.h ${LIB_DIR}semafor.h ${LIB_DIR}memoria.h ${LIB_DIR}missatge.h
PROCESS_FILES	=	${LIB_DIR}winsuport2.c ${LIB_DIR}semafor.c ${LIB_DIR}memoria.c ${LIB_DIR}missatge.c
PROCESS_OBJECTS	=	${PROCESS_FILES:.c=.o}

%.o: %.c
	gcc ${FLAGS_LIBS} -c $< -o $@

####### RULES #######
all: tennis0 tennis1 tennis2 tennis3 tennis4

tennis0: ${THREAD_OBJECTS} ${TENNIS0_SRC}tennis.h
	gcc ${FLAGS_EXE} ${TENNIS0_FILES} ${THREAD_OBJECTS} -o ${TENNIS0_EXE} -lcurses ${INCLUDE}

tennis1: ${THREAD_OBJECTS} ${TENNIS1_SRC}tennis.h
	gcc ${FLAGS_EXE} ${TENNIS1_FILES} ${THREAD_OBJECTS} -o ${TENNIS1_EXE} -lcurses ${INCLUDE}

tennis2: ${THREAD_OBJECTS} ${TENNIS2_SRC}tennis.h
	gcc ${FLAGS_EXE} ${TENNIS2_FILES} ${THREAD_OBJECTS} -o ${TENNIS2_EXE} -lcurses ${INCLUDE}

tennis3: ${PROCESS_OBJECTS} ${TENNIS3_SRC}tennis.h
	gcc ${FLAGS_EXE} ${TENNIS3_FILES} ${PROCESS_OBJECTS} -o ${TENNIS3_EXE} -lcurses ${INCLUDE}
	gcc ${FLAGS_EXE} ${TENNIS3_PROC_FILES} ${PROCESS_OBJECTS} -o ${TENNIS3_PROC_EXE} -lcurses ${INCLUDE}

tennis4: ${PROCESS_OBJECTS} ${TENNIS4_SRC}tennis.h
	gcc ${FLAGS_EXE} ${TENNIS4_FILES} ${PROCESS_OBJECTS} -o ${TENNIS4_EXE} -lcurses ${INCLUDE}
	gcc ${FLAGS_EXE} ${TENNIS4_PROC_FILES} ${PROCESS_OBJECTS} -o ${TENNIS4_PROC_EXE} -lcurses ${INCLUDE}

clean:
	rm ${THREAD_OBJECTS}
	rm ${PROCESS_OBJECTS}
	rm ${EXECS}

re: clean all

ROSE_DIR="/home/steve/local/rose-0.9.5a-11957-c_cpp_fortran"
ROSE_TUTORIAL="/home/steve/local/src/rose-0.9.5a-11957-c_cpp_fortran/tutorial"

HEADERS=memory.h journal.h Object.h ObjectList.h Unit.h AspectFunc.h FuncJP.h JPs.h joinpoint.h Aspects.h pointcut.h init.h debug.h finalise.h types.h Aspect.h Compass.h
SOURCES=memory.c journal.c Object.c ObjectList.c Unit.c AspectFunc.c FuncJP.c JPs.c joinpoint.c Aspects.c pointcut.c init.c debug.c finalise.c
OUTPUT=libcompass.so

CC=gcc
CFLAGS=-c -Wall -fPIC -g
LDFLAGS=-shared
OBJECTS=${SOURCES:.c=.o}

ROSE_BIN="${ROSE_DIR}/bin"
ROSE_CC=/usr/bin/g++ -I${ROSE_DIR}/include
ROSE_LINK=/usr/bin/g++ -L${ROSE_DIR}/lib -lrose


all: ${SOURCES} ${OUTPUT}

.c.o: ${HEADERS}
	@echo "Compile $@"
	${CC} ${CFLAGS} $< -o $@

${OUTPUT}: ${OBJECTS}
	@echo "Linking $@"
	${CC} ${LDFLAGS} ${OBJECTS} -o $@

check: all

clean:
	rm -rf ${OUTPUT} ${OBJECTS}

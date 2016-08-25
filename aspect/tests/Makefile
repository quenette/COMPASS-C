PWD=`pwd`
COMPASS_DIR="${PWD}/../src"
ROSE_DIR="/home/steve/local/rose-0.9.5a-11957-c_cpp_fortran"
ROSE_TUTORIAL="/home/steve/local/src/rose-0.9.5a-11957-c_cpp_fortran/tutorial"

SOURCES=test-memory.c test-asprintf.c test-journal.c test-Object.c\
test-ObjectList.c test-AspectFunc.c test-FuncJP.c test-pointcut.c

CC=gcc
CFLAGS=-c -Wall -g -I${COMPASS_DIR}
LDFLAGS=-L${COMPASS_DIR} -Wl,-rpath=${COMPASS_DIR} -lcompass

ROSE_BIN="${ROSE_DIR}/bin"
ROSE_CC=/usr/bin/g++ -I${ROSE_DIR}/include
ROSE_LINK=/usr/bin/g++ -L${ROSE_DIR}/lib -lrose

OBJECTS=${SOURCES:.c=.o}
TESTS=${SOURCES:.c=}

all: ${SOURCES} ${OBJECTS} ${TESTS}

.o: %.c
	@echo "Compile $@"
	${CC} ${CFLAGS} $< -o $@

%: %.o
	@echo "Linking $@"
	${CC} ${LDFLAGS} $< -o $@

check: all
	@set -e; for test in ${TESTS} ; do echo "Executing $$test"; ./$$test; done

clean:
	rm -rf ${TESTS} ${OBJECTS}

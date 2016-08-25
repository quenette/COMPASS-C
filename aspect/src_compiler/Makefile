PWD=`pwd`
COMPASS_DIR="${PWD}/../src"
ROSE_DIR="/home/steve/local/rose-0.9.5a-11957-c_cpp_fortran"
ROSE_TUTORIAL="/home/steve/local/src/rose-0.9.5a-11957-c_cpp_fortran/tutorial"

CC=gcc
CFLAGS=-Wall -g -I${COMPASS_DIR}
EDG_CFLAGS=-edg -I${COMPASS_DIR}
LDFLAGS=-L${COMPASS_DIR} -Wl,-rpath=${COMPASS_DIR} -lcompass

ROSE_BIN="${ROSE_DIR}/bin"
ROSE_CC=/usr/bin/g++ -I${ROSE_DIR}/include
ROSE_LINK=/usr/bin/g++ -L${ROSE_DIR}/lib  -Wl,-rpath=${ROSE_DIR}/lib -lrose

all: \
	pointcut1.c.svg pointcut1.c.pdf\
	pointcut2.c.svg pointcut2.c.pdf\
	pointcut3.c.svg pointcut3.c.pdf\
	pointcut4.c.svg pointcut4.c.pdf\
	pointcut5.c.svg pointcut5.c.pdf\
	coWeave\
	pointcut1.exe pointcut1.weaved.exe\
	pointcut2.exe pointcut2.weaved.exe\
	pointcut3.exe pointcut3.weaved.exe\
	pointcut4.exe pointcut4.weaved.exe\
	pointcut5.exe pointcut5.weaved.exe pointcut5.weaved.opt.exe

# Generate AST graphs as dot tree and pdf
pointcut1.c.dot: pointcut1.c
	@echo "Generating $@"
	@${ROSE_BIN}/dotGenerator ${EDG_CFLAGS} $< 

pointcut1.c.svg: pointcut1.c.dot
	@echo "Generating $@"
	@dot -Gratio=compact -Gfontname="Helvetica" $< -Tsvg -o $@ -Gordering="out"

pointcut1.c.pdf: pointcut1.c
	@echo "Generating $@"
	@${ROSE_BIN}/pdfGenerator ${EDG_CFLAGS} $<

pointcut2.c.dot: pointcut2.c
	@echo "Generating $@"
	@${ROSE_BIN}/dotGenerator ${EDG_CFLAGS} $< 

pointcut2.c.svg: pointcut2.c.dot
	@echo "Generating $@"
	@dot -Gratio=compact -Gfontname="Helvetica" $< -Tsvg -o $@ -Gordering="out"

pointcut2.c.pdf: pointcut2.c
	@echo "Generating $@"
	@${ROSE_BIN}/pdfGenerator ${EDG_CFLAGS} $<

pointcut3.c.dot: pointcut3.c
	@echo "Generating $@"
	@${ROSE_BIN}/dotGenerator ${EDG_CFLAGS} $< 

pointcut3.c.svg: pointcut3.c.dot
	@echo "Generating $@"
	@dot -Gratio=compact -Gfontname="Helvetica" $< -Tsvg -o $@ -Gordering="out"

pointcut3.c.pdf: pointcut3.c
	@echo "Generating $@"
	@${ROSE_BIN}/pdfGenerator ${EDG_CFLAGS} $<

pointcut4.c.dot: pointcut4.c
	@echo "Generating $@"
	@${ROSE_BIN}/dotGenerator ${EDG_CFLAGS} $< 

pointcut4.c.svg: pointcut4.c.dot
	@echo "Generating $@"
	@dot -Gratio=compact -Gfontname="Helvetica" $< -Tsvg -o $@ -Gordering="out"

pointcut4.c.pdf: pointcut4.c
	@echo "Generating $@"
	@${ROSE_BIN}/pdfGenerator ${EDG_CFLAGS} $<

pointcut5.c.dot: pointcut5.c
	@echo "Generating $@"
	@${ROSE_BIN}/dotGenerator ${EDG_CFLAGS} $< 

pointcut5.c.svg: pointcut5.c.dot
	@echo "Generating $@"
	@dot -Gratio=compact -Gfontname="Helvetica" $< -Tsvg -o $@ -Gordering="out"

pointcut5.c.pdf: pointcut5.c
	@echo "Generating $@"
	@${ROSE_BIN}/pdfGenerator ${EDG_CFLAGS} $<

# Rose transformers
Weaver.o: Weaver.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

FindSymbols.o: FindSymbols.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

ObtainValue.o: ObtainValue.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

CompareAssignment.o: CompareAssignment.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

InlinePrep.o: InlinePrep.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

WeaveAspectCall.o: WeaveAspectCall.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

WeavePointcutPtr.o: WeavePointcutPtr.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

coWeave.o: coWeave.cc
	@echo "Compiling $@"
	@${ROSE_CC} ${CFLAGS} -c -o $@ $<

coWeave: FindSymbols.o ObtainValue.o CompareAssignment.o coWeave.o InlinePrep.o WeaveAspectCall.o WeavePointcutPtr.o Weaver.o
	@echo "Linking $@"
	${ROSE_LINK} ${LDFLAGS} -o $@ $^ 

# Test executables...
pointcut1.weaved.c: pointcut1.c coWeave
	@echo "Generating $@"
	./coWeave -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut1.weaved.exe: pointcut1.weaved.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut1.exe: pointcut1.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut2.weaved.c: pointcut2.c coWeave
	@echo "Generating $@"
	./coWeave -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut2.weaved.exe: pointcut2.weaved.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut2.exe: pointcut2.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut3.weaved.c: pointcut3.c coWeave
	@echo "Generating $@"
	./coWeave -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut3.weaved.exe: pointcut3.weaved.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut3.exe: pointcut3.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut4.weaved.c: pointcut4.c coWeave
	@echo "Generating $@"
	./coWeave -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut4.weaved.exe: pointcut4.weaved.c
	@echo "Compile/linking $@"
	gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut4.exe: pointcut4.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut5.weaved.c: pointcut5.c coWeave
	@echo "Generating $@"
	./coWeave -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut5.weaved.exe: pointcut5.weaved.c
	@echo "Compile/linking $@"
	gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut5.exe: pointcut5.c
	@echo "Compile/linking $@"
	@gcc ${CFLAGS} ${LDFLAGS} -o $@ $<

pointcut5.weaved.opt.c: pointcut5.weaved.c
	@echo "Generating $@"
	${ROSE_BIN}/LoopProcessor -fs2 -rose:output $@ -rose:C99_only -rose:skipfinalCompileStep -rose:C_output_language ${EDG_CFLAGS} $<

pointcut5.weaved.opt.exe: pointcut5.weaved.opt.c
	@echo "Compile/linking $@"
	gcc ${CFLAGS} ${LDFLAGS} -o $@ $<


run: 
	./pointcut1.exe
	./pointcut1.weaved.exe
	./pointcut2.exe
	./pointcut2.weaved.exe
	./pointcut3.exe
	./pointcut3.weaved.exe
	./pointcut4.exe
	./pointcut4.weaved.exe
	./pointcut5.exe
	./pointcut5.weaved.exe
	./pointcut5.weaved.opt.exe

clean:
	@rm -rf pointcut1.c.dot pointcut1.c.svg pointcut1.c.pdf
	@rm -rf pointcut2.c.dot pointcut2.c.svg pointcut2.c.pdf
	@rm -rf pointcut3.c.dot pointcut3.c.svg pointcut3.c.pdf
	@rm -rf pointcut4.c.dot pointcut4.c.svg pointcut4.c.pdf
	@rm -rf pointcut5.c.dot pointcut5.c.svg pointcut5.c.pdf
	@rm -rf coWeave coWeave.o FindSymbols.o ObtainValue.o CompareAssignment.o InlinePrep.o WeaveAspectCall.o WeavePointcutPtr.o Weaver.o
	@rm -rf pointcut1.exe pointcut1.weaved.exe pointcut1.weaved.c
	@rm -rf pointcut2.exe pointcut2.weaved.exe pointcut2.weaved.c
	@rm -rf pointcut3.exe pointcut3.weaved.exe pointcut3.weaved.c
	@rm -rf pointcut4.exe pointcut4.weaved.exe pointcut4.weaved.c
	@rm -rf pointcut5.exe pointcut5.weaved.exe pointcut5.weaved.c pointcut5.weaved.opt.exe pointcut5.weaved.opt.c
	

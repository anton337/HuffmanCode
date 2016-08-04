target=huffman

FLAGS=-Wall -O3 -g0

DEBUF_FLAGS=-Wall -O0 -g3

CXX=g++

OBJECTS=main.o

%.o: %.cpp 
	${CXX} ${FLAGS} -c $< -o $@

all: ${OBJECTS}
	${CXX} ${FLAGS} -o ${target} ${OBJECTS}

debug: ${OBJECTS}
	${CXX} ${DEBUG_FLAGS} -o ${target} ${OBJECTS}

clean:
	rm -f *.o
	rm -f ${target}


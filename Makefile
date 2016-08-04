target=huffman

all:
	g++ -Wall -O3 -g0 -o ${target} main.cpp

debug:
	g++ -Wall -O0 -g3 -o ${target} main.cpp

clean:
	rm -f ${target}


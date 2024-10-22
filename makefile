all: parser.cpp
	g++ parser.cpp -o parser
clean:
	rm parser
run:
	g++ parser.cpp -o parser
	./parser
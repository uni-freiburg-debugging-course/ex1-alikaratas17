all: parser.cpp
	g++ parser.cpp -o parser
clean:
	rm -f parser my_res z3_res
run: parser.cpp
	g++ parser.cpp -o parser
	./parser
test: parser.cpp
	g++ parser.cpp -o parser
	./parser simp.smt2 > my_res
	z3 simp.smt2 > z3_res
	cmp my_res z3_res
fuzzer: fuzzer.cpp
	g++ fuzzer.cpp -o fuzzer
	./fuzzer
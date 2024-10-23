all: parser.cpp
	g++ parser.cpp -o parser
clean:
	rm -f parser my_res z3_res
run:
	g++ parser.cpp -o parser
	./parser
test:
	g++ parser.cpp -o parser
	./parser simp.smt2 > my_res
	z3 simp.smt2 > z3_res
	cmp my_res z3_res
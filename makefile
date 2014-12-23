all	: sudoku
sudoku	: sudoku.o backtrack.o
	g++ -o sudoku sudoku.o backtrack.o
sudoku.o: sudoku.cc backtrack.h
	g++ -c sudoku.cc
backtrack.o	:backtrack.h
	g++ -c backtrack.cc
clean	:
	rm -rf backtrack.o sudoku sudoku.o


all: compile
CFLAGS = -std=c++11
compile: main.cpp Lex.h Pars.h Poliz.h Exec.h
		 g++ $(CFLAGS) main.cpp Exec.h Poliz.h Pars.h Lex.h -o run.out
run: 
		./run.out
clean:
		rm run.out
